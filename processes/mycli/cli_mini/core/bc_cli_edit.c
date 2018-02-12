////////////////////////////////////////////////////////////////////////////////
// 																			  //
// 							 	头   文   件								  //
// 																			  //
////////////////////////////////////////////////////////////////////////////////

#include "bc_modules.h"

#include "bc_cli_edit.h"
#include "bc_cli_vio.h"
#include "bc_cli_hist.h"
#include "bc_cli_para.h"
#include "bc_cli_priv.h"
#include "bc_cli_core.h"
#include "bc_cli_chs.h"
#include "bc_cli_misc.h"
#include "bc_cli_vkey.h"



////////////////////////////////////////////////////////////////////////////////
//																			  //
//						外 部 变 量、 函 数 引 用							  //
//																			  //
////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////
//																			  //
//						结   构   体   宏   定   义							  //
//																			  //
////////////////////////////////////////////////////////////////////////////////

#define CLI_EDIT_DIRECTION_LEFT 	1
#define CLI_EDIT_DIRECTION_RIGHT 	2

/* (x, y ) is used to record the coordinate of the cursor; the left side of the prompt is (0, 0);
 * 0 <= x; 0 <= y <= CLI_EDIT_LINE_LEN;
 * x increase 1 for a ASCII (2 for a Chinese char), y increase 1 per line;
 * if x >= CLI_EDIT_LINE_LEN - 1, y increase 1 and x is set to 0;
 */
typedef struct
{
	UI16_T x;
	UI16_T y;
} CLI_EDIT_COORDINATE_T;

typedef struct
{
	UI32_T 					prompt_len;    	/* length of the shell prompt */
	CLI_INPUT_MODE_T 		input_mode;
	cli_output_mode_e 		output_mode;
	HANDLE_T 				hvio; 			/* create by vio */
	BOOL_T 					use_history; 	/* TRUE: use history function; FALSE: not use history function */
	CLI_CHARSET_T 			charset;
	I8_T 					*pbuf; 			/* from CLI_EDIT_Get_Line */
	UI32_T 					buf_len; 		/* from CLI_EDIT_Get_Line, pbuf[buflen] */
	UI32_T 					edit_len; 		/* position of the cursor(afther shell prompt to cursor) */
	CLI_EDIT_COORDINATE_T 	cursor;
	UI32_T 					display_len; 	/* after shell prompt to the end of the line */
} CLI_EDITSTATE_T;

#define CLI_EDIT_Is_BUF_ENOUGH(p_edit_state,remainder_ch) (p_edit_state->display_len + remainder_ch <= p_edit_state->buf_len)



////////////////////////////////////////////////////////////////////////////////
//																			  //
//							全 局 变 量 定 义								  //
//																			  //
////////////////////////////////////////////////////////////////////////////////

static unsigned int 	l_mdl_id = BC_MODULE_CLI_KERNEL_EDIT;



////////////////////////////////////////////////////////////////////////////////
//																			  //
//								函  数  声  明								  //
//																			  //
////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////
//																			  //
//								函  数  定  义								  //
//																			  //
////////////////////////////////////////////////////////////////////////////////

static void cli2_edit_get_cursor_coordinate(
	IN OUT CLI_EDITSTATE_T *p_edit_state,
	OUT CLI_EDIT_COORDINATE_T *cursor)
{
	I32_T i;
	I32_T size;

	CLI_DBG_ENTER(l_mdl_id);

#if 0
	if (p_edit_state->edit_len > p_edit_state->display_len)
	{
		p_edit_state->edit_len = p_edit_state->display_len;
	}
#endif

	cursor->x = p_edit_state->prompt_len;
	cursor->y = 0;

	for (i = 0; i < p_edit_state->edit_len; i += size)
	{
		size = CLI_CHARSET_Get_NextCharSize(p_edit_state->charset, (UI8_T *)p_edit_state->pbuf, i);
		if (size > 1)
		{
			cursor->x = cursor->x + 2;
		}
		else
		{
			cursor->x = cursor->x + 1;
		}

		if (cursor->x >= CLI_EDIT_LINE_LEN)
		{
			cursor->y++;
			cursor->x = cursor->x - CLI_EDIT_LINE_LEN;
		}
	}

	// p_edit_state->cursor.x = cursor->x;
	// p_edit_state->cursor.y = cursor->y;

	CLI_DBG_LEAVE(l_mdl_id);
	return;
}



static void cli2_edit_redraw_cursor(	IN OUT CLI_EDITSTATE_T *p_edit_state)
{
	I32_T i;
	I32_T size = 0;
	CLI_EDIT_COORDINATE_T prev_cursor;
	CLI_EDIT_COORDINATE_T cursor;

	cursor.x = p_edit_state->prompt_len;
	cursor.y = 0;
	prev_cursor.x = p_edit_state->prompt_len;
	prev_cursor.y = 0;

	for (i = 0; i < p_edit_state->edit_len; i += size)
	{
		prev_cursor.x = cursor.x;
		prev_cursor.y = cursor.y;
		size = CLI_CHARSET_Get_NextCharSize(p_edit_state->charset, (UI8_T *)p_edit_state->pbuf, i);

		if (size > 1)
		{
			cursor.x = cursor.x + 2;
		}
		else
		{
			cursor.x = cursor.x + 1;
		}

		if (cursor.x >= CLI_EDIT_LINE_LEN)
		{
			cursor.y++;
			cursor.x = cursor.x - CLI_EDIT_LINE_LEN;
		}
	}

	p_edit_state->cursor.x = cursor.x;
	p_edit_state->cursor.y = cursor.y;

	if ((cursor.x == 0) && (prev_cursor.x != 0))
	{
		CLI_VIO_Putc(p_edit_state->hvio, '\r');
		CLI_VIO_Putc(p_edit_state->hvio, '\n');
	}
}



static BOOL_T cli2_edit_move_left (IN OUT CLI_EDITSTATE_T *p_edit_state)
{
	CLI_EDIT_COORDINATE_T new_cursor;
	CLI_EDIT_COORDINATE_T old_cursor;
	I32_T x_move = 0;
	I32_T y_move = 0;
	I32_T size = 0;

	// xprintk ("\r\n cli2_edit_move_left");
	size = CLI_CHARSET_Get_PrevCharSize(p_edit_state->charset, (UI8_T *)p_edit_state->pbuf, p_edit_state->edit_len);
	if (size == 0)
	{
		return FALSE;
	}

	cli2_edit_get_cursor_coordinate(p_edit_state, &old_cursor);
	p_edit_state->edit_len -= size;
	cli2_edit_get_cursor_coordinate(p_edit_state, &new_cursor);
	y_move = new_cursor.y - old_cursor.y;
	// x_move = (CLI_EDIT_LINE_LEN + p_edit_state->cursor.x - new_cursor.x) % CLI_EDIT_LINE_LEN;
	x_move = new_cursor.x - old_cursor.x;

	// xprintk ("\r\n (x, y) = (%d, %d); new(x, y) = (%d, %d)", p_edit_state->cursor.x, p_edit_state->cursor.y, new_cursor.x, new_cursor.y);
	// xprintk ("\r\n x_move = %d, y_move = %d, size = %d", x_move, y_move, size);

	if (CLI_VIO_Move_Cursor_XY(p_edit_state->hvio, x_move, y_move))
	{
		/* update current cursor's position */
		p_edit_state->cursor.x = new_cursor.x;
		p_edit_state->cursor.y = new_cursor.y;
	}

	// cli2_edit_redraw_cursor (p_edit_state);
	return FALSE;
}


static BOOL_T cli2_edit_move_right(IN OUT CLI_EDITSTATE_T *p_edit_state)
{
	CLI_EDIT_COORDINATE_T new_cursor;
	CLI_EDIT_COORDINATE_T old_cursor;
	I32_T x_move = 0;
	I32_T y_move = 0;
	I32_T size = 0;

	// xprintk ("\r\n cli2_edit_move_right");
	if (p_edit_state->edit_len >= p_edit_state->display_len)
	{
		return FALSE;
	}

	size = CLI_CHARSET_Get_NextCharSize(p_edit_state->charset, (UI8_T *)p_edit_state->pbuf, p_edit_state->edit_len);
	if (size == 0)
	{
		return FALSE;
	}

	cli2_edit_get_cursor_coordinate(p_edit_state, &old_cursor);

	p_edit_state->edit_len += size;
	cli2_edit_get_cursor_coordinate(p_edit_state, &new_cursor);
	y_move = new_cursor.y - old_cursor.y;
	// x_move = (CLI_EDIT_LINE_LEN + p_edit_state->cursor.x - new_cursor.x) % CLI_EDIT_LINE_LEN;
	x_move = new_cursor.x - old_cursor.x;
	// xprintk ("\r\n (x, y) = (%d, %d); new(x, y) = (%d, %d)", p_edit_state->cursor.x, p_edit_state->cursor.y, new_cursor.x, new_cursor.y);
	// xprintk ("\r\n x_move = %d, y_move = %d", x_move, y_move);


	if (CLI_VIO_Move_Cursor_XY(p_edit_state->hvio, x_move, y_move))
	{
		/* update current cursor's position */
		p_edit_state->cursor.x = new_cursor.x;
		p_edit_state->cursor.y = new_cursor.y;
	}
	// cli2_edit_redraw_cursor (p_edit_state);

	return FALSE;
}



/*******************************************************************************
 *
 * FUNCTION NAME:
 *   cli2_edit_move_cursor
 *
 * DESCRIPTION:
 *   move the cursor right or left
 *
 * INPUT:
 *   p_edit_state: edit state
 *   movement: the cursor movement ,> 0 rigth, < 0 left
 *       if movement is too large , it move best effort
 *
 * OUTPUT:
 *   p_edit_state: change the current position of cursor
 *
 * RETURN:
 *   TRUE: move success
 *   FALSE: move failure
 *
 * NOTES:
 *   NA
 *
 * HISTORY:
 *
*******************************************************************************/
static BOOL_T cli2_edit_move_cursor(IN OUT CLI_EDITSTATE_T *p_edit_state, IN I32_T movement)
{
	I32_T i;

	if (movement > 0)
	{
		for (i = 0; i < movement; i++)
		{
			cli2_edit_move_right(p_edit_state);
		}
	}
	else if (movement < 0)
	{
		movement = -movement;
		for (i = 0; i < movement; i++)
		{
			cli2_edit_move_left (p_edit_state);
		}
	}

	return TRUE;
}



/*******************************************************************************
 *
 * FUNCTION NAME:
 *   cli2_edit_move_home
 *
 * DESCRIPTION:
 *   move the cursor home
 *
 * INPUT:
 *   p_edit_state: edit state
 *
 * OUTPUT:
 *   p_edit_state: change the current position of cursor
 *
 * RETURN:
 *   TRUE: move success
 *   FALSE: move failure
 *
 * NOTES:
 *   NA
 *
 * HISTORY:
 *
*******************************************************************************/
static BOOL_T cli2_edit_move_home(IN CLI_EDITSTATE_T *p_edit_state)
{
	CLI_EDIT_COORDINATE_T old_cursor;
	CLI_EDIT_COORDINATE_T new_cursor;
	I32_T x_move = 0;
	I32_T y_move = 0;

	cli2_edit_get_cursor_coordinate(p_edit_state, &old_cursor);
	new_cursor.x = p_edit_state->prompt_len;
	new_cursor.y = 0;

	y_move = new_cursor.y - old_cursor.y;
	x_move = new_cursor.x - old_cursor.x;
	p_edit_state->edit_len = 0;

	if (CLI_VIO_Move_Cursor_XY(p_edit_state->hvio, x_move, y_move))
	{
		p_edit_state->cursor.x = new_cursor.x;
		p_edit_state->cursor.y = new_cursor.y;
		return TRUE;
	}

	return FALSE;
}



/*******************************************************************************
 *
 * FUNCTION NAME:
 *   cli2_edit_move_end
 *
 * DESCRIPTION:
 *   move the cursor end
 *
 * INPUT:
 *   p_edit_state: edit state
 *
 * OUTPUT:
 *   p_edit_state: change the current position of cursor
 *
 * RETURN:
 *   TRUE : move success
 *   FALSE: move failure
 *
 * NOTES:
 *   NA
 *
 * HISTORY:
 *
*******************************************************************************/
static BOOL_T cli2_edit_move_end(IN OUT CLI_EDITSTATE_T *p_edit_state)
{
	CLI_EDIT_COORDINATE_T old_cursor;
	CLI_EDIT_COORDINATE_T new_cursor;
	I32_T x_move = 0;
	I32_T y_move = 0;

	cli2_edit_get_cursor_coordinate(p_edit_state, &old_cursor);
	p_edit_state->edit_len = p_edit_state->display_len;

	cli2_edit_get_cursor_coordinate(p_edit_state, &new_cursor);
	y_move = new_cursor.y - old_cursor.y;
	x_move = new_cursor.x - old_cursor.x;

	if (CLI_VIO_Move_Cursor_XY(p_edit_state->hvio, x_move, y_move))
	{
		p_edit_state->cursor.x = new_cursor.x;
		p_edit_state->cursor.y = new_cursor.y;
		return TRUE;
	}

	return FALSE;
}



static BOOL_T cli2_edit_reprint_cmd(
	IN CLI_EDITSTATE_T *p_edit_state,
	IN I32_T clear_len)
{
	I32_T i;
	I32_T old_edit_len;
	I32_T size = 0;
	CLI_CHARSET_T charset;
	CLI_EDIT_COORDINATE_T old_cursor;
	CLI_EDIT_COORDINATE_T new_cursor;
	CLI_EDIT_COORDINATE_T prev_cursor;

	old_edit_len = p_edit_state->edit_len;
	old_cursor.x = p_edit_state->cursor.x;
	old_cursor.y = p_edit_state->cursor.y;
	new_cursor.x = p_edit_state->cursor.x;
	new_cursor.y = p_edit_state->cursor.y;
	prev_cursor.x = p_edit_state->cursor.x;
	prev_cursor.y = p_edit_state->cursor.y;

	for (i = 1; i <= clear_len; i++)
	{
		p_edit_state->pbuf[p_edit_state->display_len] = ' ';
		p_edit_state->display_len++;
		if (p_edit_state->display_len > p_edit_state->buf_len - 2)
		{
			clear_len = i;
			break;
		}
	}

	p_edit_state->pbuf[p_edit_state->display_len] = '\0';
	for (i = old_edit_len; i < p_edit_state->display_len; i += size)
	{
		size = CLI_CHARSET_Get_NextCharSize(p_edit_state->charset, (UI8_T *)p_edit_state->pbuf, p_edit_state->edit_len);
		// xprintk ("\r\n ###size(%d)###\r\n", size);
		if (size == 0)
		{
			break;
		}

		if (size == 1)
		{
			charset = CLI_CHARSET_ASCII;
		}
		else
		{
			charset = p_edit_state->charset;
		}
		// xprintk ("\r\n ###charset(%d)###\r\n", charset);

		switch (charset)
		{
		case CLI_CHARSET_GB2312: /* fall through */
		case CLI_CHARSET_GBK:
			if (p_edit_state->output_mode == CLI_OUTPUT_PASSWORD)
			{
				if (p_edit_state->edit_len >= p_edit_state->display_len - clear_len)
				{
					CLI_VIO_Putc(p_edit_state->hvio, ' ');
					CLI_VIO_Putc(p_edit_state->hvio, ' ');
				}
				else
				{
					CLI_VIO_Putc(p_edit_state->hvio, '*');
					CLI_VIO_Putc(p_edit_state->hvio, '*');
				}
			}
			else
			{
				CLI_VIO_Putc(p_edit_state->hvio, p_edit_state->pbuf[p_edit_state->edit_len]);
				CLI_VIO_Putc(p_edit_state->hvio, p_edit_state->pbuf[p_edit_state->edit_len + 1]);
			}
			break;

		case CLI_CHARSET_ASCII: /* fall through */
		default:
			if (p_edit_state->output_mode == CLI_OUTPUT_PASSWORD)
			{
				if (p_edit_state->edit_len >= p_edit_state->display_len - clear_len)
				{
					CLI_VIO_Putc(p_edit_state->hvio, ' ');
				}
				else
				{
					CLI_VIO_Putc(p_edit_state->hvio, '*');
				}
			}
			else
			{
				CLI_VIO_Putc(p_edit_state->hvio, p_edit_state->pbuf[p_edit_state->edit_len]);
			}
			break;
		}

		p_edit_state->edit_len += size;
		cli2_edit_get_cursor_coordinate(p_edit_state, &new_cursor);
		p_edit_state->cursor.x = new_cursor.x;
		p_edit_state->cursor.y = new_cursor.y;

		if (new_cursor.x == 0)
		{
			CLI_VIO_Putc(p_edit_state->hvio, '\r');
			CLI_VIO_Putc(p_edit_state->hvio, '\n');
		}

		prev_cursor.x = new_cursor.x;
		prev_cursor.y = new_cursor.y;

		UNUSED(prev_cursor);

		// cli2_edit_move_cursor(p_edit_state, 1);
		// cli2_edit_redraw_cursor (p_edit_state);
	}

	// cli2_edit_move_cursor(p_edit_state, -clear_len);
	// p_edit_state->display_len -= size;
	// cli2_edit_get_cursor_coordinate(p_edit_state, &new_cursor);
	p_edit_state->display_len -= clear_len;
	p_edit_state->pbuf[p_edit_state->display_len] = '\0';

	if (CLI_VIO_Move_Cursor_XY(p_edit_state->hvio, old_cursor.x - p_edit_state->cursor.x, old_cursor.y - p_edit_state->cursor.y))
	{
		p_edit_state->edit_len = old_edit_len;
		p_edit_state->cursor.x = old_cursor.x;
		p_edit_state->cursor.y = old_cursor.y;
	}

	// p_edit_state->cursor.x = old_cursor.x;
	// p_edit_state->cursor.y = old_cursor.y;

	return TRUE;
}



static BOOL_T cli2_edit_insert_char(
	IN OUT CLI_EDITSTATE_T *p_edit_state,
	IN UI8_T *buf,
	IN CLI_CHARSET_T charset)
{
	CLI_EDIT_COORDINATE_T new_cursor;

	switch (charset)
	{
	case CLI_CHARSET_GB2312: /* fall through */
	case CLI_CHARSET_GBK:
		if (p_edit_state->display_len > p_edit_state->buf_len - 3)
		{
			/* input buffer full */
			return FALSE;
		}

		memmove(p_edit_state->pbuf + p_edit_state->edit_len + 2,
				p_edit_state->pbuf + p_edit_state->edit_len,
				p_edit_state->display_len - p_edit_state->edit_len + 1);

		p_edit_state->pbuf[p_edit_state->edit_len] = buf[0];
		p_edit_state->pbuf[p_edit_state->edit_len + 1] = buf[1];
		p_edit_state->display_len += 2;
		cli2_edit_reprint_cmd(p_edit_state, 0);
		cli2_edit_move_right(p_edit_state);
		// cli2_edit_redraw_cursor (p_edit_state);
		break;

	case CLI_CHARSET_ASCII: /* fall through */
	default:
		if (p_edit_state->display_len > p_edit_state->buf_len - 2)
		{
			/* input buffer full */
			return FALSE;
		}

		memmove(p_edit_state->pbuf + p_edit_state->edit_len + 1,
				p_edit_state->pbuf + p_edit_state->edit_len,
				p_edit_state->display_len - p_edit_state->edit_len + 1);

		p_edit_state->pbuf[p_edit_state->edit_len] = buf[0];
		p_edit_state->display_len += 1;
		cli2_edit_reprint_cmd(p_edit_state, 0);
		cli2_edit_move_right(p_edit_state);
		// cli2_edit_redraw_cursor (p_edit_state);

		break;
	}

	cli2_edit_get_cursor_coordinate(p_edit_state, &new_cursor);
	p_edit_state->cursor.x = new_cursor.x;
	p_edit_state->cursor.y = new_cursor.y;

	return TRUE;
}



static BOOL_T cli2_edit_replace_char(
	IN OUT CLI_EDITSTATE_T *p_edit_state,
	IN UI8_T *buf,
	IN CLI_CHARSET_T charset)
{
	I32_T size;
	CLI_EDIT_COORDINATE_T new_cursor;

	switch (charset)
	{
	case CLI_CHARSET_GB2312: /* fall through */
	case CLI_CHARSET_GBK:
		size = CLI_CHARSET_Get_NextCharSize(p_edit_state->charset, (UI8_T *)p_edit_state->pbuf, p_edit_state->edit_len);
		if (size == 1)
		{
			if (!CLI_EDIT_Is_BUF_ENOUGH(p_edit_state, 2)) // if (p_edit_state->display_len > p_edit_state->buf_len - 2)
			{
				return FALSE;
			}
			memmove(p_edit_state->pbuf + p_edit_state->edit_len + 2,
					p_edit_state->pbuf + p_edit_state->edit_len + 1,
					p_edit_state->display_len - p_edit_state->edit_len);
			p_edit_state->display_len += 1;
		}

		/* store the input character */
		p_edit_state->pbuf[p_edit_state->edit_len] = buf[0];
		p_edit_state->pbuf[p_edit_state->edit_len + 1] = buf[1];

		/* print the input character */
		if (p_edit_state->output_mode == CLI_OUTPUT_PASSWORD)
		{
			CLI_VIO_Putc(p_edit_state->hvio, '*');
			CLI_VIO_Putc(p_edit_state->hvio, '*');
		}
		else
		{
			CLI_VIO_Putc(p_edit_state->hvio, buf[0]);
			CLI_VIO_Putc(p_edit_state->hvio, buf[1]);
		}

		cli2_edit_redraw_cursor (p_edit_state);
		p_edit_state->edit_len += 2;
		if (size == 1)
		{
			cli2_edit_reprint_cmd(p_edit_state, 0);
		}

		break;

	case CLI_CHARSET_ASCII: /* fall through */
	default:
		size = CLI_CHARSET_Get_NextCharSize(p_edit_state->charset, (UI8_T *)p_edit_state->pbuf, p_edit_state->edit_len);
		if (size == 2)
		{
			if (!CLI_EDIT_Is_BUF_ENOUGH(p_edit_state, 2)) // if (p_edit_state->display_len > p_edit_state->buf_len - 2)
			{
				return FALSE;
			}

			memmove(p_edit_state->pbuf + p_edit_state->edit_len + 1,
					p_edit_state->pbuf + p_edit_state->edit_len + 2,
					p_edit_state->display_len - p_edit_state->edit_len - 1);
			p_edit_state->display_len -= 1;
		}

		/* store the input character */
		p_edit_state->pbuf[p_edit_state->edit_len] = buf[0];

		/* print the input character */
		if (p_edit_state->output_mode == CLI_OUTPUT_PASSWORD)
		{
			CLI_VIO_Putc(p_edit_state->hvio, '*');
		}
		else
		{
			CLI_VIO_Putc(p_edit_state->hvio, buf[0]);
		}

		// cli2_edit_move_right(p_edit_state);
		cli2_edit_redraw_cursor (p_edit_state);
		p_edit_state->edit_len += 1;
		if (size == 2)
		{
			cli2_edit_reprint_cmd(p_edit_state, 1);
		}

		break;
	}

	cli2_edit_get_cursor_coordinate(p_edit_state, &new_cursor);
	p_edit_state->cursor.x = new_cursor.x;
	p_edit_state->cursor.y = new_cursor.y;

	return TRUE;
}



static BOOL_T cli2_edit_append_char(
	IN OUT CLI_EDITSTATE_T *p_edit_state,
	IN UI8_T *buf,
	IN CLI_CHARSET_T charset)
{
	CLI_EDIT_COORDINATE_T new_cursor;

	switch (charset)
	{
	case CLI_CHARSET_GB2312: /* fall through */
	case CLI_CHARSET_GBK:
		if (p_edit_state->buf_len - p_edit_state->display_len < 3)
		{
			return FALSE;
		}

		p_edit_state->pbuf[p_edit_state->edit_len] = buf[0];
		p_edit_state->pbuf[p_edit_state->edit_len + 1] = buf[1];
		p_edit_state->pbuf[p_edit_state->edit_len + 2] = '\0';
		if (p_edit_state->output_mode == CLI_OUTPUT_PASSWORD)
		{
			CLI_VIO_Putc(p_edit_state->hvio, '*');
			CLI_VIO_Putc(p_edit_state->hvio, '*');
		}
		else
		{
			CLI_VIO_Putc(p_edit_state->hvio, buf[0]);
			CLI_VIO_Putc(p_edit_state->hvio, buf[1]);
		}

		p_edit_state->edit_len += 2;
		p_edit_state->display_len += 2;

		break;

	case CLI_CHARSET_ASCII: /* fall through */
	default:
		if (p_edit_state->buf_len - p_edit_state->display_len < 2)
		{
			return FALSE;
		}

		p_edit_state->pbuf[p_edit_state->edit_len] = buf[0];
		p_edit_state->pbuf[p_edit_state->edit_len + 1] = '\0';
		if (p_edit_state->output_mode == CLI_OUTPUT_PASSWORD)
		{
			CLI_VIO_Putc(p_edit_state->hvio, '*');
		}
		else
		{
			CLI_VIO_Putc(p_edit_state->hvio, buf[0]);
		}

		p_edit_state->edit_len += 1;
		p_edit_state->display_len += 1;

		break;
	}

	cli2_edit_redraw_cursor (p_edit_state);
	cli2_edit_get_cursor_coordinate(p_edit_state, &new_cursor);
	p_edit_state->cursor.x = new_cursor.x;
	p_edit_state->cursor.y = new_cursor.y;

	return TRUE;
}



static BOOL_T cli2_edit_print_char(
	IN OUT CLI_EDITSTATE_T *p_edit_state,
	IN UI8_T *buf,
	IN CLI_CHARSET_T charset)
{
#if 0
	if (CLI_CHARSET_Is_CursorInsideChar(p_edit_state->charset,
		p_edit_state->pbuf, p_edit_state->edit_len))
	{
		return FALSE;
	}
#endif

	if (p_edit_state->display_len <= p_edit_state->edit_len)
	{
		// p_edit_state->edit_len = p_edit_state->display_len;
		return cli2_edit_append_char(p_edit_state, buf, charset);
	}
	else
	{
		if (p_edit_state->input_mode == CLI_INPUT_INSERT)
		{
			return cli2_edit_insert_char(p_edit_state, buf, charset);
		}
		else
		{
			return cli2_edit_replace_char(p_edit_state, buf, charset);
		}
	}
}



/*******************************************************************************
 *
 * FUNCTION NAME:
 *   cli2_edit_print_cmd
 *
 * DESCRIPTION:
 *   put the input cmd to screen
 *
 * INPUT:
 *   p_edit_state: edit state
 *   cmd         : the input string
 *   bEnd        : TRUE  -- cursor will state the end position of cmd
 *                 FALSE -- cursor will state original position
 *
 * OUTPUT:
 *   p_edit_state: change the current position of cursor and display length
 *
 * RETURN:
 *   TRUE : print success
 *   FALSE: print failure
 *
 * NOTES:
 *   NA
 *
 * HISTORY:
 *
*******************************************************************************/
static BOOL_T cli2_edit_print_cmd(IN OUT CLI_EDITSTATE_T *p_edit_state, IN I8_T *cmd, IN BOOL_T bEnd)
{
	CLI_DBG_ENTER(l_mdl_id);

	UI32_T length;
	UI32_T length2;
	I32_T clear_len;
	CLI_EDIT_COORDINATE_T old_cursor;
	UI32_T old_edit_len;

	if (cmd == NULL)
	{
		CLI_DBG_LEAVE(l_mdl_id);
		return FALSE;
	}

	old_edit_len = p_edit_state->edit_len;
	length = p_edit_state->display_len - p_edit_state->edit_len;
	length2 = strlen(cmd);

	if (length2 > (p_edit_state->buf_len - p_edit_state->edit_len - 1))
	{
		CLI_DBG_LEAVE(l_mdl_id);
		return FALSE;
	}

	old_cursor.x = p_edit_state->cursor.x;
	old_cursor.y = p_edit_state->cursor.y;

	strcpy(p_edit_state->pbuf + p_edit_state->edit_len, cmd);
	p_edit_state->display_len = p_edit_state->edit_len + length2;
	if (length2 < length)
	{
		clear_len = length - length2;
	}
	else
	{
		clear_len = 0;
	}

	// xprintk ("\r\n clear_len = %d\r\n", clear_len);
	// xprintk ("\r\n display_len = %d\r\n", p_edit_state->display_len);
	// xprintk ("\r\n edit_len = %d\r\n", p_edit_state->edit_len);
	// xprintk ("\r\n #####################################\r\n");
	// xprintk ("\r\n #####################################\r\n");
	// xprintk ("\r\n %s\r\n", p_edit_state->pbuf);
	// xprintk ("\r\n #####################################\r\n");
	// xprintk ("\r\n #####################################\r\n");
	// xprintk ("\r\n #####################################\r\n");

	cli2_edit_reprint_cmd(p_edit_state, clear_len);

	/* move cursor to the end position of cmd */
	if (bEnd == TRUE)
	{
		cli2_edit_move_end(p_edit_state);
	}
#if 0
	else
	{
		I32_T x_move = 0;
		I32_T y_move = 0;

		y_move = old_cursor.y - p_edit_state->cursor.y;
		x_move = old_cursor.x - p_edit_state->cursor.x;

		if (CLI_VIO_Move_Cursor_XY(p_edit_state->hvio, x_move, y_move))
		{
			p_edit_state->cursor.x = old_cursor.x;
			p_edit_state->cursor.y = old_cursor.y;
		}
	}
#endif

	UNUSED(old_cursor);
	UNUSED(old_edit_len);

	CLI_DBG_CPL(l_mdl_id, "prompt_len=%u,cmd=%s\r\n", p_edit_state->prompt_len, cmd);
	CLI_DBG_LEAVE(l_mdl_id);
	return TRUE;
}



static BOOL_T cli2_edit_delete_right(IN CLI_EDITSTATE_T *p_edit_state)
{
	I32_T size;
	I32_T clear_len;

	if (p_edit_state->edit_len == p_edit_state->display_len)
	{
		return FALSE;
	}

	size = CLI_CHARSET_Get_NextCharSize(p_edit_state->charset, (UI8_T *)p_edit_state->pbuf, p_edit_state->edit_len);
	if (size == 0)
	{
		return FALSE;
	}

	clear_len = size;
	memmove(p_edit_state->pbuf + p_edit_state->edit_len,
			p_edit_state->pbuf + p_edit_state->edit_len + size,
			p_edit_state->display_len - p_edit_state->edit_len - size + 1);
	p_edit_state->display_len -= size;

	return cli2_edit_reprint_cmd(p_edit_state, clear_len);
}



static BOOL_T cli2_edit_delete_left (IN CLI_EDITSTATE_T *p_edit_state)
{
	if (p_edit_state->edit_len)
	{
		cli2_edit_move_cursor(p_edit_state, -1);
		return cli2_edit_delete_right(p_edit_state);
	}

	return FALSE;
}



/*******************************************************************************
 *
 * FUNCTION NAME:
 *   cli2_edit_clean_line
 *
 * DESCRIPTION:
 *   clean line charater
 *
 * INPUT:
 *   p_edit_state: edit state
 *
 * OUTPUT:
 *   p_edit_state: change the current position of cursor and display length
 *
 * RETURN:
 *   TRUE : clean success
 *   FALSE: clean failure
 *
 * NOTES:
 *   NA
 *
 * HISTORY:
 *
*******************************************************************************/
static void cli2_edit_clean_line(IN OUT CLI_EDITSTATE_T *p_edit_state)
{
	if (p_edit_state->display_len == 0)
	{
		return;
	}

	/* move the cursor to the beginning of input cmd */
	cli2_edit_move_home(p_edit_state);
	/* delete the input cmd */
	cli2_edit_print_cmd(p_edit_state, "", FALSE);
	return;
}



/*******************************************************************************
 *
 * FUNCTION NAME:
 *   cli2_edit_get_prevhist
 *
 * DESCRIPTION:
 *   get the previous history cmd, print it and update current cmd
 *
 * INPUT:
 *   p_edit_state: edit state
 *   h_hist: handle of the history for using history module
 *
 * OUTPUT:
 *   p_edit_state: change the current position of cursor and display length
 *
 * RETURN:
 *   TRUE: get history cmd success and print it, update current cmd
 *   FALSE: get history cmd failure
 *
 * NOTES:
 *   NA
 *
 * HISTORY:
 *
*******************************************************************************/
static BOOL_T cli2_edit_get_prevhist(IN HANDLE_T h_hist, IN OUT CLI_EDITSTATE_T *p_edit_state)
{
	I8_T *prevhist;

	/* when input password cli2_edit_get_prevhist can't be used */
	if ((p_edit_state->output_mode == CLI_OUTPUT_PASSWORD) || (h_hist == NULL))
	{
		return FALSE;
	}

	prevhist = CLI_HIST_GetPrevItem(h_hist);
	/* don't get history */
	if (prevhist == NULL)
	{
		return FALSE;
	}

	if (strcmp(p_edit_state->pbuf, prevhist) == 0)
	{
		prevhist = CLI_HIST_GetPrevItem(h_hist);
		/* don't get history */
		if (prevhist == NULL)
		{
			return FALSE;
		}
	}

	/* print history cmd and update current cmd */
	cli2_edit_move_home(p_edit_state);
	cli2_edit_print_cmd(p_edit_state, prevhist, TRUE);

	return TRUE;
}



/*******************************************************************************
 *
 * FUNCTION NAME:
 *   cli2_edit_get_nexthist
 *
 * DESCRIPTION:
 *   get the next history cmd, print it and update current cmd
 *
 * INPUT:
 *   p_edit_state: edit state
 *   h_hist: handle of the history for using history module
 *
 * OUTPUT:
 *   p_edit_state: change the current position of cursor and display length
 *
 * RETURN:
 *   TRUE: get history cmd success and print it, update current cmd
 *   FALSE: get history cmd failure
 *
 * NOTES:
 *   NA
 *
 * HISTORY:
 *
*******************************************************************************/
static BOOL_T cli2_edit_get_nexthist(IN HANDLE_T h_hist, IN CLI_EDITSTATE_T * p_edit_state)
{
	I8_T *nexthist;

	/* when input password cli2_edit_get_nexthist can't be used */
	if ((p_edit_state->output_mode == CLI_OUTPUT_PASSWORD) || (h_hist == NULL))
	{
		return FALSE;
	}

	nexthist = CLI_HIST_GetNextItem(h_hist);
	/* don't get history */
	if (nexthist == NULL)
	{
		return FALSE;
	}

	if (strcmp(p_edit_state->pbuf, nexthist) == 0)
	{
		nexthist = CLI_HIST_GetNextItem(h_hist);
		if (nexthist == NULL)
		{
			return FALSE;
		}
	}

	/* print history cmd and update current cmd */
	cli2_edit_move_home(p_edit_state);
	cli2_edit_print_cmd(p_edit_state, nexthist, TRUE);

	return TRUE;
}



/*******************************************************************************
 *
 * FUNCTION NAME:
 *   CLI_EDIT_Create
 *
 * DESCRIPTION:
 *   create a handle for using edit module
 *
 * INPUT:
 *   hvio: the handle for using vio module
 *
 * OUTPUT:
 *   NA
 *
 * RETURN:
 *   pointer: the handle for using edit module
 *
 * NOTES:
 *   NA
 *
 * HISTORY:
 *
*******************************************************************************/
HANDLE_T CLI_EDIT_Create(IN HANDLE_T hvio)
{
	CLI_EDITSTATE_T *hedit;

	hedit = (CLI_EDITSTATE_T *)CLI_MISC_Calloc(1, sizeof(CLI_EDITSTATE_T));
	if (!hedit)
	{
		return (HANDLE_T)hedit;
	}

	/* init handle */
	hedit->hvio = hvio;
	hedit->input_mode = CLI_INPUT_INSERT;
	hedit->output_mode = CLI_OUTPUT_NORMAL;
	hedit->use_history = TRUE;
	hedit->charset = CLI_DEFAULT_CHARSET;
	hedit->cursor.x = 0;
	hedit->cursor.y = 0;

	return (HANDLE_T) hedit;
}



/*******************************************************************************
 *
 * FUNCTION NAME:
 *   CLI_EDIT_Destroy
 *
 * DESCRIPTION:
 *   Destroy the handle and free memory
 *
 * INPUT:
 *   hedit: the handle for using edit module
 *
 * OUTPUT:
 *   NA
 *
 * RETURN:
 *   NA
 *
 * NOTES:
 *   NA
 *
 * HISTORY:
 *
*******************************************************************************/
void CLI_EDIT_Destroy(IN HANDLE_T hedit)
{
	cli_misc_free(hedit);

	return;
}



/*******************************************************************************
 *
 * FUNCTION NAME:
 *   CLI_EDIT_Get_InputMode
 *
 * DESCRIPTION:
 *   Get the input_mode
 *
 * INPUT:
 *   hedit: the handle for using edit module
 *
 * OUTPUT:
 *   NA
 *
 * RETURN:
 *   mode: input mode
 *
 * NOTES:
 *   NA
 *
 * HISTORY:
 *
*******************************************************************************/
CLI_INPUT_MODE_T CLI_EDIT_Get_InputMode(IN HANDLE_T hedit)
{
	CLI_EDITSTATE_T *p_edit_state;

	p_edit_state = (CLI_EDITSTATE_T *) hedit;

	return p_edit_state->input_mode;
}



/*******************************************************************************
 *
 * FUNCTION NAME:
 *   CLI_EDIT_Get_OutputMode
 *
 * DESCRIPTION:
 *   get the output_mode
 *
 * INPUT:
 *   hedit: the handle for using edit module
 *
 * OUTPUT:
 *   NA
 *
 * RETURN:
 *   mode: output mode
 *
 * NOTES:
 *   NA
 *
 * HISTORY:
 *
*******************************************************************************/
CLI_INPUT_MODE_T CLI_EDIT_Get_OutputMode(IN HANDLE_T hedit)
{
	CLI_EDITSTATE_T *p_edit_state;

	p_edit_state = (CLI_EDITSTATE_T *) hedit;

	return p_edit_state->output_mode;
}



/*******************************************************************************
 *
 * FUNCTION NAME:
 *   CLI_EDIT_Set_InputMode
 *
 * DESCRIPTION:
 *   Set the input_mode
 *
 * INPUT:
 *   mode  : input mode
 *   hedit : the handle for using edit module
 *
 * OUTPUT:
 *   hedit : the handle for using edit module
 *
 * RETURN:
 *   TRUE  : set success
 *   FALSE : set failure
 *
 * NOTES:
 *   NA
 *
 * HISTORY:
 *
*******************************************************************************/
BOOL_T CLI_EDIT_Set_InputMode(IN OUT HANDLE_T hedit, IN CLI_INPUT_MODE_T mode)
{
	CLI_EDITSTATE_T *p_edit_state;

	p_edit_state = (CLI_EDITSTATE_T *) hedit;
	p_edit_state->input_mode = mode;

	return TRUE;
}



/*******************************************************************************
 *
 * FUNCTION NAME:
 *   CLI_EDIT_Get_PromptLen
 *
 * DESCRIPTION:
 *   Get the length of the prompt
 *
 * INPUT:
 *   hedit : the handle for using edit module
 *
 * OUTPUT:
 *   NA
 *
 * RETURN:
 *   length of the prompt
 *
 * NOTES:
 *   NA
 *
 * HISTORY:
 *
*******************************************************************************/
UI32_T CLI_EDIT_Get_PromptLen (IN OUT HANDLE_T hedit)
{
	CLI_EDITSTATE_T *p_edit_state;

	p_edit_state = (CLI_EDITSTATE_T *) hedit;

	return p_edit_state->prompt_len;
}



/*******************************************************************************
 *
 * FUNCTION NAME:
 *   CLI_EDIT_Set_PromptLen
 *
 * DESCRIPTION:
 *   Set the length of the prompt
 *
 * INPUT:
 *   hedit      : the handle for using edit module
 *   prompt_len : length of the prompt
 *
 * OUTPUT:
 *   hedit      : the handle for using edit module
 *
 * RETURN:
 *   TRUE       : set success
 *   FALSE      : set failure
 *
 * NOTES:
 *   NA
 *
 * HISTORY:
 *
*******************************************************************************/
BOOL_T CLI_EDIT_Set_PromptLen (IN OUT HANDLE_T hedit, IN I32_T prompt_len)
{
	CLI_EDITSTATE_T *p_edit_state;

	p_edit_state = (CLI_EDITSTATE_T *) hedit;
	p_edit_state->prompt_len = prompt_len;

	return TRUE;
}



/*******************************************************************************
 *
 * FUNCTION NAME:
 *   CLI_EDIT_Set_OutputMode
 *
 * DESCRIPTION:
 *   Set the output_mode
 *
 * INPUT:
 *   hedit : the handle for using edit module
 *   mode  : output mode
 *
 * OUTPUT:
 *   hedit : the handle for using edit module
 *
 * RETURN:
 *   TRUE  : set success
 *   FALSE : set failure
 *
 * NOTES:
 *   NA
 *
 * HISTORY:
 *
*******************************************************************************/
BOOL_T CLI_EDIT_Set_OutputMode(IN OUT HANDLE_T hedit, IN cli_output_mode_e mode)
{
	CLI_EDITSTATE_T *p_edit_state;

	p_edit_state = (CLI_EDITSTATE_T *) hedit;
	p_edit_state->output_mode = mode;

	return TRUE;
}



/*******************************************************************************
 *
 * FUNCTION NAME:
 *   CLI_EDIT_Set_HistoryStatus
 *
 * DESCRIPTION:
 *   set the history status
 *
 * INPUT:
 *   hedit       : the handle for using edit module
 *   use_history : TRUE: enable history; FALSE: disable history
 *
 * OUTPUT:
 *   hedit       : the handle for using edit module
 *
 * RETURN:
 *   NA
 *
 * NOTES:
 *   NA
 *
 * HISTORY:
 *
*******************************************************************************/
void CLI_EDIT_Set_HistoryStatus (IN OUT HANDLE_T hedit, IN BOOL_T use_history)
{
	CLI_EDITSTATE_T *p_edit_state;

	p_edit_state = (CLI_EDITSTATE_T *) hedit;
	p_edit_state->use_history = use_history;

	return;
}



/**************************************************************************
FUNCTION NAME:
	CLI_EDIT_Match_Tab

DESCRIPTION:
	Checks all consecutive <Tab>s, and complete the command.

INPUT:
	hedit		: the handle for using edit module
	view		: current path
	level		: current user level
	helplist	: struct pointer of help list

OUTPUT:
	helplist	: the result of complement current word
	p_edit_state: new edit state
 	p_edit_state: new command state

RETURN:
	TRUE		: the command is complemented, continue to parse in this line.
	FALSE		: the command don't need to complement,
				  the command prompt and input command should be print in new line.

NOTES:
	NA

HISTORY:

**************************************************************************/
BOOL_T CLI_EDIT_Match_Tab (
	IN HANDLE_T 				hedit,
	IN I8_T 					*view,
	IN UI32_T 					level,
	IN OUT CLI_PRIV_HELPLIST_T *helplist)
{
	CLI_EDITSTATE_T *p_edit_state;
	I32_T tab_word_len, most_match_len, temp_len;
	I32_T keyword_cnt;
	I8_T *src_ptr, *cmp_ptr;
	I8_T attach_word[80];
	I32_T i;

	/* init cmd state */
	p_edit_state = (CLI_EDITSTATE_T *)hedit;

	cli2_edit_move_end(p_edit_state);
	tab_word_len = 0;
	if (p_edit_state->edit_len && (p_edit_state->pbuf[p_edit_state->edit_len - 1] != ' '))
	{
		// complete the last word in user command
		while (tab_word_len < p_edit_state->edit_len)
		{
			if (p_edit_state->pbuf[p_edit_state->edit_len-tab_word_len - 1] != ' ')
			{
				tab_word_len++;
			}
			else
			{
				break;
			}
		}

		helplist->nexthelp = FALSE;
		helplist->local_help = FALSE;
		CLI_DBG_CPL(l_mdl_id, "helplist->local_help=%d\r\n", helplist->local_help);
		if (CLI_CORE_Get_Help(helplist, view, p_edit_state->pbuf, level) >= 0)
		{
			keyword_cnt = helplist->count - helplist->para_count;
			if (strcmp(helplist->name[helplist->count - 1], "<cr>") == 0)
			{
				keyword_cnt--;
			}

			if (keyword_cnt == 1)
			{
				/* best fit match a keyword */
				cli2_edit_print_cmd(p_edit_state, helplist->name[helplist->para_count] + tab_word_len, TRUE);
				cli2_edit_print_char(p_edit_state, (UI8_T *)" ", CLI_CHARSET_ASCII);
				return TRUE;
			}
			else if (keyword_cnt > 1)
			{
				/* find out the match chars length and print these chars */
				most_match_len = strlen(helplist->name[helplist->para_count]);
				for (i = 1; i < keyword_cnt; i++)
				{
					temp_len = 0;
					src_ptr = helplist->name[helplist->para_count];
					cmp_ptr = helplist->name[helplist->para_count + i];
					while (*src_ptr && *cmp_ptr)
					{
						if (*src_ptr++ == *cmp_ptr++)
						{
							temp_len++;
						}
						else
						{
							break;
						}
					}

					if (temp_len < most_match_len)
					{
						most_match_len = temp_len;
					}
				}

				/* get the match chars and print them, already printed chars will be skiped, so reduce the tab_word_len */
				if (most_match_len - tab_word_len > 0)
				{
					CLI_MISC_Assert(most_match_len - tab_word_len < sizeof(attach_word));
					for (i = 0; i < most_match_len - tab_word_len; i++)
					{
						/* get the chars need to print, already printed chars will be skiped, so add the tab_word_len */
						attach_word[i] = *(helplist->name[helplist->para_count] + tab_word_len + i);
					}

					attach_word[i] = '\0';
					cli2_edit_print_cmd(p_edit_state, attach_word, TRUE);

					return TRUE;
				}
			}
		}
	}

	cli2_edit_move_end(p_edit_state);
	CLI_VIO_Puts(p_edit_state->hvio, "\r\n", 2);

	return FALSE;
}



/*******************************************************************************
 *
 * FUNCTION NAME:
 *   CLI_EDIT_Get_Char
 *
 * DESCRIPTION:
 *   Get char from terminal
 *
 * INPUT:
 *   hedit       : handle for edit module
 *   timeout_sec : timeout second
 *
 * OUTPUT:
 *   NA
 *
 * RETURN:
 *   >0 : char
 *   -1 : IO closed
 *   -2 : timeout
 *   -3 : noaction(the input function is null)
 *
 * NOTES:
 *   NA
 *
 * HISTORY:
 *   2009-Aug-28 by wangbin -- Create
 *
*******************************************************************************/
I32_T CLI_EDIT_Get_Char(IN HANDLE_T hedit, IN I32_T timeout_sec)
{
	CLI_EDITSTATE_T *p_edit_state;
	I32_T timeout;

	if (timeout_sec <= 0)
	{
		timeout = 3600 * 1000;
	}
	else
	{
		timeout = timeout_sec * 1000;
	}

	p_edit_state = (CLI_EDITSTATE_T *) hedit;

	return CLI_VIO_Getc(p_edit_state->hvio, timeout);
}



/*******************************************************************************
 *
 * FUNCTION NAME:
 *   CLI_EDIT_Get_Line
 *
 * DESCRIPTION:
 *   Get the input cmd and control keys
 *
 * INPUT:
 *   buf_len     : the length of buffer
 *   hedit       : the handle for using edit module
 *   h_hist      : handle of the history for using history module
 *   pbuf        : pointer of buffer for storing input cmd
 *   timeout_sec : time out second
 *
 * OUTPUT:
 *   hedit       : the handle for using edit module
 *   pbuf        : pointer of input cmd string
 *
 * RETURN:
 *   CLI_VKEY_IO_CLOSED   : IO closed
 *   CLI_VKEY_IO_TIMEOUT  : timeout
 *   CLI_VKEY_IO_NOACTION : noaction(the input function is null)
 *   CLI_VKEY_IO_EXITCMD  : exit cmd
 *   CLI_VKEY_CTRL_Z      : Ctrl-Z pressed
 *   '?'                  : Get help for the last word in the line
 *   '-'                  : Get help for the word after the line (" ?" or "-?" pressed)
 *   '\n'                 : Input CR and get input command success
 *
 * NOTES:
 *   NA
 *
 * HISTORY:
 *
*******************************************************************************/
I32_T CLI_EDIT_Get_Line(
	IN OUT HANDLE_T hedit,
	IN HANDLE_T h_hist,
	IN OUT I8_T *pbuf,
	IN UI32_T buf_len,
	IN I32_T timeout_sec,
	IN I32_T previous_ch)
{
	CLI_DBG_ENTER(l_mdl_id);

	CLI_EDITSTATE_T *p_edit_state;
	I32_T rx_int;
	I32_T timeout;
	UI8_T buf_ch[10];
	CLI_EDIT_COORDINATE_T new_cursor;
	BOOL_T help_flag = TRUE;

	/* init cmd state */
	p_edit_state = (CLI_EDITSTATE_T *) hedit;
	p_edit_state->pbuf = pbuf;
	p_edit_state->buf_len = buf_len;
	p_edit_state->edit_len = strlen(pbuf);
	p_edit_state->display_len = strlen(pbuf);
#if 0
	if (p_edit_state->edit_len == 0)
	{
		p_edit_state->cursor.x = p_edit_state->prompt_len;
		p_edit_state->cursor.y = 0;
	}
#else
	cli2_edit_get_cursor_coordinate(p_edit_state, &new_cursor);
	p_edit_state->cursor.x = new_cursor.x;
	p_edit_state->cursor.y = new_cursor.y;
#endif
	// cli2_edit_move_end (p_edit_state);

	if (timeout_sec <= 0)
	{
		timeout = 0; /* never timeout */
	}
	else
	{
		timeout = timeout_sec * 1000;
	}

	for (;;)
	{
		if (previous_ch == 0)
		{
			rx_int = CLI_VIO_Getc(p_edit_state->hvio, timeout);
		}
		else
		{
			rx_int = previous_ch;
			previous_ch = 0;
		}

		CLI_DBG_CPL(l_mdl_id, "[rx_int=\"%c\",%d,0x%X]\r\n", rx_int, rx_int, rx_int);

		if (rx_int == CLI_VKEY_CTRL_V)
		{
			help_flag = !help_flag;
		}
		else if (rx_int != '?')
		{
			help_flag = TRUE;
		}

		switch (rx_int)
		{
		case CLI_VKEY_IO_CLOSED:
		case CLI_VKEY_IO_NOACTION:
		case CLI_VKEY_IO_EXITCMD:
		case CLI_VKEY_IO_NOTIFY:
			CLI_DBG_LEAVE(l_mdl_id);
			return rx_int;

		case CLI_VKEY_IO_TIMEOUT:
			/* if time out second = 0 never time out */
			if (timeout_sec > 0)
			{
				CLI_DBG_LEAVE(l_mdl_id);
				return rx_int;
			}
			break;

		case CLI_VKEY_CTRL_ENTER:
		case CLI_VKEY_CR:
			cli2_edit_move_end(p_edit_state);
			CLI_VIO_Puts(p_edit_state->hvio, "\r\n", 2);
			CLI_DBG_LEAVE(l_mdl_id);
			return '\n';

		case CLI_VKEY_LEFT:
			cli2_edit_move_cursor(p_edit_state, -1);
			break;

		case CLI_VKEY_RIGHT:
			cli2_edit_move_cursor(p_edit_state, 1);
			break;

		case CLI_VKEY_INSERT:
		case CLI_VKEY_CTRL_R:
			if (p_edit_state->input_mode == CLI_INPUT_NORMAL)
			{
				p_edit_state->input_mode = CLI_INPUT_INSERT;
			}
			else
			{
				p_edit_state->input_mode = CLI_INPUT_NORMAL;
			}
			break;

		case CLI_VKEY_CTRL_A:
		case CLI_VKEY_HOME:
			cli2_edit_move_home(p_edit_state);
			break;

		case CLI_VKEY_CTRL_K:
			cli2_edit_print_cmd(p_edit_state, "", TRUE);
			break;

		case CLI_VKEY_CTRL_E:
		case CLI_VKEY_END:
			cli2_edit_move_end(p_edit_state);
			break;

		case CLI_VKEY_DELETE:
		case CLI_VKEY_DEL:
		case CLI_VKEY_CTRL_D:
			cli2_edit_delete_right(p_edit_state);
			break;

		case CLI_VKEY_BACKSPACE:
			cli2_edit_delete_left(p_edit_state);
			break;

		case CLI_VKEY_ESC:
		case CLI_VKEY_CTRL_U:
			cli2_edit_clean_line(p_edit_state);
			break;

		case CLI_VKEY_CTRL_Z:
			cli2_edit_move_end(p_edit_state);
			CLI_VIO_Puts(p_edit_state->hvio, "\r\n", 2);
			return CLI_VKEY_CTRL_Z;
			break;

		case CLI_VKEY_F1:
		case CLI_VKEY_F2:
		case CLI_VKEY_F3:
		case CLI_VKEY_F4:
		case CLI_VKEY_F5:
		case CLI_VKEY_F6:
		case CLI_VKEY_F7:
		case CLI_VKEY_F8:
		case CLI_VKEY_F9:
		case CLI_VKEY_F10:
		case CLI_VKEY_F11:
		case CLI_VKEY_F12:
			break;

		case CLI_VKEY_CTRL_P: /* fall through */
		case CLI_VKEY_UP:
			if (p_edit_state->use_history)
			{
				if (p_edit_state->output_mode == CLI_OUTPUT_PASSWORD)
				{
					break;
				}

				if (p_edit_state->output_mode == CLI_OUTPUT_WITHOUT_SPECIAL_CHAR)
				{
					break;
				}

				cli2_edit_get_prevhist(h_hist, p_edit_state);
			}
			break;

		case CLI_VKEY_CTRL_N: /* fall through */
		case CLI_VKEY_DOWN:
			if (p_edit_state->use_history)
			{
				if (p_edit_state->output_mode == CLI_OUTPUT_PASSWORD)
				{
					break;
				}

				if (p_edit_state->output_mode == CLI_OUTPUT_WITHOUT_SPECIAL_CHAR)
				{
					break;
				}

				cli2_edit_get_nexthist(h_hist, p_edit_state);
			}
			break;

		case CLI_VKEY_TAB:
			if (p_edit_state->output_mode == CLI_OUTPUT_PASSWORD)
			{
				break;
			}

			if (p_edit_state->output_mode == CLI_OUTPUT_WITHOUT_SPECIAL_CHAR)
			{
				break;
			}
			cli2_edit_move_end(p_edit_state);
			CLI_DBG_LEAVE(l_mdl_id);
			return '\t';

		case '?':
			if (p_edit_state->output_mode == CLI_OUTPUT_NORMAL)
			{
				if (help_flag)
				{
					cli2_edit_move_end(p_edit_state);
					CLI_VIO_Puts(p_edit_state->hvio, "?\r\n", 3);
					if ((p_edit_state->edit_len > 1) && (p_edit_state->pbuf[p_edit_state->edit_len - 1] == ' '))
					{
						CLI_DBG_LEAVE(l_mdl_id);
						return '-';
					}

					CLI_DBG_LEAVE(l_mdl_id);
					return '?';
				}
				else
				{
					help_flag = TRUE;
				}
			}

		default: /* input a normal char */
			if ((rx_int < 0x20) || (rx_int >= 0x100))
			{
				break;
			}

			buf_ch[0] = (UI8_T)rx_int;
			if (CLI_CHARSET_Is_AsciiChar(buf_ch[0]))
			{
				cli2_edit_print_char(p_edit_state, buf_ch, CLI_CHARSET_ASCII);
			}
			else if (p_edit_state->charset == CLI_CHARSET_GB2312)
			{
				if (CLI_CHARSET_Is_Gb2312FirstChar(buf_ch[0]))
				{
					rx_int = CLI_VIO_Getc(p_edit_state->hvio, timeout);
					CLI_DBG_CPL(l_mdl_id, "[rx_int=\"%c\",%d,0x%X]\r\n", rx_int, rx_int, rx_int);
					if ((rx_int < 0x20) || (rx_int > CLI_VKEY_EDIT_KEY_BASE))
					{
						if (rx_int == CLI_VKEY_IO_NOTIFY)
						{
							CLI_DBG_LEAVE(l_mdl_id);
							return rx_int;
						}
						break;
					}

					buf_ch[1] = (UI8_T)rx_int;
					if (CLI_CHARSET_Is_Gb2312SecondChar(buf_ch[1]))
					{
						cli2_edit_print_char(p_edit_state, buf_ch, CLI_CHARSET_GB2312);
					}
				}
			}
			else if (p_edit_state->charset == CLI_CHARSET_GBK)
			{
				if (CLI_CHARSET_Is_GbkFirstChar(buf_ch[0]))
				{
					rx_int = CLI_VIO_Getc(p_edit_state->hvio, timeout);
					CLI_DBG_CPL(l_mdl_id, "[rx_int=\"%c\",%d,0x%X]\r\n", rx_int, rx_int, rx_int);
					if ((rx_int < 0x20) || (rx_int > CLI_VKEY_EDIT_KEY_BASE))
					{
						if (rx_int == CLI_VKEY_IO_NOTIFY)
						{
							CLI_DBG_LEAVE(l_mdl_id);
							return rx_int;
						}

						break;
					}

					buf_ch[1] = (UI8_T)rx_int;
					if (CLI_CHARSET_Is_GbkSecondChar(buf_ch[1]))
					{
						cli2_edit_print_char(p_edit_state, buf_ch, CLI_CHARSET_GBK);
					}
				}
			}
			break;
		}
	}
}



/*******************************************************************************
 *
 * FUNCTION NAME:
 *   CLI_EDIT_Get_Line_WithBreak
 *
 * DESCRIPTION:
 *   Get the input chars until trigger chars.
 *
 * INPUT:
 *   hedit       : the handle for using edit module
 *   pbuf        : pointer of buffer for storing input cmd
 *   buf_len     : the length of buffer
 *   timeout_sec : time out second
 *
 * OUTPUT:
 *   hedit       : the handle for using edit module
 *   pbuf        : pointer of input cmd string
 *
 * RETURN:
 *   CLI_VKEY_IO_CLOSED   : IO closed
 *   CLI_VKEY_IO_TIMEOUT  : timeout
 *   CLI_VKEY_IO_NOACTION : noaction(the input function is null)
 *   CLI_VKEY_IO_EXITCMD  : exit cmd
 *   trigger char
 *
 * NOTES:
 *   NA
 *
 * HISTORY:
 *
*******************************************************************************/
I32_T CLI_EDIT_Get_Line_WithBreak(
	IN OUT HANDLE_T hedit,
	IN OUT I8_T *pbuf,
	IN UI32_T buf_len,
	IN I32_T timeout_sec,
	IN I32_T *trigger)
{
	CLI_EDITSTATE_T *p_edit_state;
	I32_T rx_int;
	I32_T timeout;
	I32_T i;
	UI8_T buf_ch[10];
	CLI_EDIT_COORDINATE_T new_cursor;

	/* init cmd state */
	p_edit_state = (CLI_EDITSTATE_T *) hedit;
	p_edit_state->pbuf = pbuf;
	p_edit_state->buf_len = buf_len;
	p_edit_state->edit_len = strlen(pbuf);
	p_edit_state->display_len = strlen(pbuf);
	
	cli2_edit_get_cursor_coordinate(p_edit_state, &new_cursor);
	p_edit_state->cursor.x = new_cursor.x;
	p_edit_state->cursor.y = new_cursor.y;

	if (timeout_sec <= 0)
	{
		timeout = 0; /* never timeout */
	}
	else
	{
		timeout = timeout_sec * 1000;
	}

	for (;;)
	{
		rx_int = CLI_VIO_Getc(p_edit_state->hvio, timeout);
		CLI_DBG_CPL(l_mdl_id, "[rx_int=\"%c\",%d,0x%X]\r\n", rx_int, rx_int, rx_int);

		switch (rx_int)
		{
		case CLI_VKEY_IO_CLOSED:
		case CLI_VKEY_IO_EXITCMD:
		case CLI_VKEY_IO_NOTIFY:
			return rx_int;

		case CLI_VKEY_IO_TIMEOUT:
			/* if time out second = 0 never time out */
			if (timeout_sec > 0)
			{
				return rx_int;
			}
			break;

		case CLI_VKEY_LEFT:
			cli2_edit_move_cursor(p_edit_state, -1);
			break;

		case CLI_VKEY_RIGHT:
			cli2_edit_move_cursor(p_edit_state, 1);
			break;

		// case CLI_VKEY_INSERT:
		case CLI_VKEY_CTRL_R:
			if (p_edit_state->input_mode == CLI_INPUT_NORMAL)
			{
				p_edit_state->input_mode = CLI_INPUT_INSERT;
			}
			else
			{
				p_edit_state->input_mode = CLI_INPUT_NORMAL;
			}
			break;

		case CLI_VKEY_CTRL_A:
		case CLI_VKEY_HOME:
			cli2_edit_move_home(p_edit_state);
			break;

		case CLI_VKEY_CTRL_K:
			cli2_edit_print_cmd(p_edit_state, "", TRUE);
			break;

		case CLI_VKEY_CTRL_E:
		case CLI_VKEY_END:
			cli2_edit_move_end(p_edit_state);
			break;

		case CLI_VKEY_DELETE:
		case CLI_VKEY_DEL:
		case CLI_VKEY_CTRL_D:
			cli2_edit_delete_right(p_edit_state);
			break;

		case CLI_VKEY_BACKSPACE:
			cli2_edit_delete_left(p_edit_state);
			break;

		case CLI_VKEY_ESC:
			cli2_edit_clean_line(p_edit_state);
			break;

		case CLI_VKEY_F1:
		case CLI_VKEY_F2:
		case CLI_VKEY_F3:
		case CLI_VKEY_F4:
		case CLI_VKEY_F5:
		case CLI_VKEY_F6:
		case CLI_VKEY_F7:
		case CLI_VKEY_F8:
		case CLI_VKEY_F9:
		case CLI_VKEY_F10:
		case CLI_VKEY_F11:
		case CLI_VKEY_F12:
			break;

		default:
			/* check the trigger chars */
			for (i = 0; trigger[i] != 0; i++)
			{
				if (trigger[i] == rx_int)
				{
					cli2_edit_move_end(p_edit_state);
					return rx_int;
				}
			}
			
			/* input a normal char */
			if ((rx_int < 0x20) || (rx_int >= 0x100))
			{
				break;
			}

			buf_ch[0] = (UI8_T)rx_int;
			if (CLI_CHARSET_Is_AsciiChar(buf_ch[0]))
			{
				cli2_edit_print_char(p_edit_state, buf_ch, CLI_CHARSET_ASCII);
			}
			else if (p_edit_state->charset == CLI_CHARSET_GB2312)
			{
				if (CLI_CHARSET_Is_Gb2312FirstChar(buf_ch[0]))
				{
					rx_int = CLI_VIO_Getc(p_edit_state->hvio, timeout);
					CLI_DBG_CPL(l_mdl_id, "[rx_int=\"%c\",%d,0x%X]\r\n", rx_int, rx_int, rx_int);
					if ((rx_int < 0x20) || (rx_int > CLI_VKEY_EDIT_KEY_BASE))
					{
						if (rx_int == CLI_VKEY_IO_NOTIFY)
						{
							return rx_int;
						}
						break;
					}
					buf_ch[1] = (UI8_T)rx_int;
					if (CLI_CHARSET_Is_Gb2312SecondChar(buf_ch[1]))
					{
						cli2_edit_print_char(p_edit_state, buf_ch, CLI_CHARSET_GB2312);
					}
				}
			}
			else if (p_edit_state->charset == CLI_CHARSET_GBK)
			{
				if (CLI_CHARSET_Is_GbkFirstChar(buf_ch[0]))
				{
					rx_int = CLI_VIO_Getc(p_edit_state->hvio, timeout);
					CLI_DBG_CPL(l_mdl_id, "[rx_int=\"%c\",%d,0x%X]\r\n", rx_int, rx_int, rx_int);
					if ((rx_int < 0x20) || (rx_int > CLI_VKEY_EDIT_KEY_BASE))
					{
						if (rx_int == CLI_VKEY_IO_NOTIFY)
						{
							return rx_int;
						}
						break;
					}
					buf_ch[1] = (UI8_T)rx_int;
					if (CLI_CHARSET_Is_GbkSecondChar(buf_ch[1]))
					{
						cli2_edit_print_char(p_edit_state, buf_ch, CLI_CHARSET_GBK);
					}
				}
			}
			break;
		}
	}
}



/* End of file */

