/*******************************************************************************
 * Copyright(C) 2008, Kingtype Networks Corporation. All rights reserved.
 *
 * This is unpublished proprietary source code of Kingtype Networks Corporation.
 *
 * The copyright notice above does not evidence any actual or intended
 *  publication of such source code.
*******************************************************************************/
/*==============================================================================
 * MODULE NAME: Kingtype CLI, bc_cli
 * FILE NAME  : bc_cli_core.c
 * VERSION    : 1.0.0
 * DESCRIPTION:
 * NOTES:
 *
 * HISTORY:
 * -----------------------------------------------------------------------------
 * Version 1.0 -- 2009-Jul-15 by wangbin
 * [NEW FEATURE]
 * <DESCRIPTION> Create
 *============================================================================*/

#include "bc_modules.h"
#include "bc_cli_type.h"
#include "bc_cli_db.h"
#include "bc_cli_core.h"
#include "bc_cli_para.h"
#include "bc_cli_parse.h"
#include "bc_cli_misc.h"

#include "bc_cli.h"



typedef struct
{
	I8_T *name;
	I8_T *help_en;
	I8_T *help_local;
	I32_T name_len;
	I32_T help_en_len;
	I32_T help_local_len;
} CLI_CORE_WORD_T;



static unsigned int 	l_mdl_id = BC_MODULE_CLI_KERNEL_CORE;



/*******************************************************************************
 *
 * FUNCTION NAME:
 *   cli2_core_get_syncmd_word
 *
 * DESCRIPTION:
 *   get a word from command string.
 *
 * INPUT:
 *   cmd: the cmd string
 *   word: pointer to get the current command word msg
 *
 * OUTPUT:
 *   word: current command word msg
 *
 * RETURN:
 *   NULL: get word failed.
 *   not NULL: point to the next word.
 *
 * NOTES:
 *   None
 *
 * HISTORY:
 *
*******************************************************************************/
static I8_T *cli2_core_get_syncmd_word(
	IN I8_T *cmd,
	OUT CLI_CORE_WORD_T *word)
{
	/* skip spaces */
	while (*cmd == ' ')
	{
		cmd++;
	}

	/* get a word and insert to db */
	if ((*cmd == '\0') || (*cmd == '#'))
	{
		return NULL;
	}

	word->name = cmd;
	while (*cmd && (*cmd != ' ') && (*cmd != '#'))
	{
		cmd++;
	}

	word->name_len = cmd - word->name;
	word->help_en = NULL;
	word->help_en_len = 0;
	word->help_local = NULL;
	word->help_local_len = 0;

	if (*cmd == '#')
	{
		cmd++;
		if (*cmd)
		{
			word->help_en = (I8_T *)cmd;
		}

		while (*cmd && *cmd != '#')
		{
			cmd++;
		}

		if (*cmd == '#')
		{
			word->help_en_len = cmd - word->help_en;
			cmd++;
		}
		else
		{
			return NULL;
		}
	}

	if (*cmd == '#')
	{
		cmd++;
		if (*cmd)
		{
			word->help_local = (I8_T *)cmd;
		}

		while (*cmd && *cmd != '#')
		{
			cmd++;
		}

		if (*cmd == '#')
		{
			word->help_local_len = cmd - word->help_local;
			cmd++;
		}
		else
		{
			return NULL;
		}
	}

	/* skip spaces */
	while (*cmd == ' ')
	{
		cmd++;
	}

	return cmd;
}



/*******************************************************************************
 *
 * FUNCTION NAME:
 *   cli2_core_check_first_parameter_is_string
 *
 * DESCRIPTION:
 *   check the command's first parameter is a string-like parameter (string or keyword)
 *
 * INPUT:
 *   cmd_node: the command node
 *
 * OUTPUT:
 *   None
 *
 * RETURN:
 *   TRUE: first parameter is a string-like parameter
 *   FALSE: no parameter or not string-like parameter
 *
 * NOTES:
 *   None
 *
 * HISTORY:
 *
*******************************************************************************/
static BOOL_T cli2_core_check_first_parameter_is_string(
	IN CLI_CMD_NODE_T *cmd_node)
{
	I32_T count;

	count = 0;

	if (cmd_node->para == NULL)
	{
		return FALSE;
	}

	CLI_Para_Check_FirstUnitIsStringLike(cmd_node->para, &count);
	if (count > 0)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

/*******************************************************************************
 *
 * FUNCTION NAME:
 *   cli2_core_check_same_name_path_exist
 *
 * DESCRIPTION:
 *   check have same path name with the command name under given path
 *
 * INPUT:
 *   cmd_node: the command node
 *   path: the given path
 *
 * OUTPUT:
 *   None
 *
 * RETURN:
 *  TRUE: a path exist which have same name with the command name
 *  FALSE: no same name path exist
 *
 * NOTES:
 *   None
 *
 * HISTORY:
 *
*******************************************************************************/
static BOOL_T cli2_core_check_same_name_path_exist(
	IN CLI_CMD_NODE_T *path,
	IN CLI_CMD_NODE_T *cmd_node)
{
	UI32_T len;

	/* check can found a path with the given name */
	len = strlen(cmd_node->name);
	return CLI_DB_Find_Entry(path, CLI_TYPE_CMD, cmd_node->name, len, FALSE, NULL) ? TRUE : FALSE;
}

/*******************************************************************************
 *
 * FUNCTION NAME:
 *   cli2_core_get_childs_min_level
 *
 * DESCRIPTION:
 *   get the min-level of childs belong to the path by loop every node of path's sub-tree
 *
 * INPUT:
 *   path_node: the path
 *
 * OUTPUT:
 *   None
 *
 * RETURN:
 *  -1: no child exist
 *  0-255: the min level
 *
 * NOTES:
 *   None
 *
 * HISTORY:
 *
*******************************************************************************/
static I32_T cli2_core_get_childs_min_level(IN CLI_CMD_NODE_T *path_node)
{
	CLI_CMD_NODE_T *child;
	UI32_T min_level = (UI32_T)(-1);

	/* loop all child under the path */
	child = CLI_DB_Get_FirstEntry(path_node);
	while (child)
	{
		if (min_level > (UI32_T) child->level)
		{
			min_level = child->level;
		}

		child = CLI_DB_Get_NextEntry(path_node, child);
	}

	return (I32_T)(min_level);
}

/*******************************************************************************
 *
 * FUNCTION NAME:
 *   cli2_core_destory_path
 *
 * DESCRIPTION:
 *   destory the path (include command) from command DB:
 *     loop every path
 *     -> destory from DB (delete from tree and free the node buffer)
 *     -> get the min level of childs belong to path
 *     -> if no child exist: remove this node
 *       else if level not same: update the level
 *       else: break(same level means other command have the same path)
 *
 * INPUT:
 *   path_node: the path
 *   path_num: the path number (include root path)
 *   cmd_node: the command node
 *
 * OUTPUT:
 *   None
 *
 * RETURN:
 *   None
 *
 * NOTES:
 *   None
 *
 * HISTORY:
 *
*******************************************************************************/
static void cli2_core_destory_path(IN CLI_CMD_NODE_T **path_node,
	IN UI32_T path_num, IN CLI_CMD_NODE_T *cmd_node)
{
	I32_T level;

	/* count to array index */
	path_num--;

	if (cmd_node)
	{
		/* delete the command node */
		CLI_DB_Destory_Entry(path_node[path_num], (CLI_CMD_NODE_T *) cmd_node);
	}

	/* delete each path if no child exist under that path, root path node not need destory */
	while (path_num)
	{
		/* get the min level of path's childs */
		level = cli2_core_get_childs_min_level(path_node[path_num]);
		if ((level == -1) && path_node[path_num]->callback == NULL)
		{
			/* delete the help information buffer first */
			if (path_node[path_num]->help_alloc_flag && path_node[path_num]->help_en)
			{
				cli_misc_free(path_node[path_num]->help_en);
			}

			/* no child exist -> delete this path node from it's parent path node */
			CLI_DB_Destory_Entry(path_node[path_num - 1], (CLI_CMD_NODE_T *) path_node[path_num]);
		}
		else
		{
			/* child exist -> update the level */
			if (path_node[path_num]->level < level)
			{
				/* old level < min level of childs -> update the level */
				path_node[path_num]->level = (UI8_T) level;
			}
			else
			{
				break;
			}
		}

		/* backward one path */
		path_num--;
	}
}



/*******************************************************************************
 *
 * FUNCTION NAME:
 *   cli2_core_create_view
 *
 * DESCRIPTION:
 *   Create or Find a view node in db.
 *
 * INPUT:
 *   level: level of the view
 *   view: name of the view
 *
 * OUTPUT:
 *   path: result path nodes.
 *   path_num: path nodes already be created or found.
 *
 * RETURN:
 *   TRUE: find or create path db success.
 *   FALSE: find or create path db failed.
 *
 * NOTES:
 *   None
 *
 * HISTORY:
 *   2009-Aug-05 by wangbin -- Create
 *
*******************************************************************************/
static BOOL_T cli2_core_create_view
	(
	IN const I8_T 		*view,
	IN UI8_T 			level,
	OUT CLI_CMD_NODE_T **path,
	OUT UI32_T 			*path_num
	)
{
	CLI_DBG_ENTER(l_mdl_id);

	BOOL_T 				is_new;
	const I8_T 			*name;
	I32_T 				name_len;
	CLI_CMD_NODE_T 	*path_node;
	UI32_T 				num = 0;

	path_node = CLI_ROOT_PATH;
	path[num++] = CLI_ROOT_PATH;
	if (NULL == view)
	{
		*path_num = num;
		return TRUE;
	}

	CLI_DBG_TAG(l_mdl_id);
	name = view;
	while (1)
	{
		CLI_DBG_TAG(l_mdl_id);
		if ((*view == '/') || (*view == '\0'))
		{
			name_len = view - name;
			CLI_DBG_TAG(l_mdl_id);
			path_node = CLI_DB_Find_Entry(path_node, CLI_TYPE_VIEW, name, name_len, TRUE, &is_new);
			CLI_DBG_TAG(l_mdl_id);
			if (NULL == path_node)
			{
				CLI_DBG_TAG(l_mdl_id);
				/* memory allocated fail! */
				cli2_core_destory_path(path, num, NULL);
				*path_num = 0;
				return FALSE;
			}

			if ((is_new) || (path_node->level > level))
			{
				/* new path or old level > this level, set this path to the min level */
				path_node->level = level;
			}

			path[num++] = path_node;
			name = view + 1;
		}

		CLI_DBG_TAG(l_mdl_id);
		if (*view == '\0')
		{
			break;
		}

		view++;
	}

	*path_num = num;

	CLI_DBG_LEAVE(l_mdl_id);
	return TRUE;
}



/*******************************************************************************
 *
 * FUNCTION NAME:
 *   cli2_core_create_cmd
 *
 * DESCRIPTION:
 *   create the command with its path:
 *     loop every word
 *     -> if word is path: find/create the path(word+' ' or '/'),
 *        else: create command, update output pointer and return
 *
 * INPUT:
 *  cmd_str: the pointer of command string pointer
 *  level: the access level of command
 *
 * OUTPUT:
 *  cmd: the new string position of command parameter or end('\0')
 *  path: the command's path-nodes (include the root path)
 *  path_num: the number of path (include the root path)
 *
 * RETURN:
 *  pointer: the command node pointer that auto-allocated
 *  NULL: error found:	empty-string or invalid-word or memory-alloc-fail or command-existed
 *
 * NOTES:
 *   None
 *
 * HISTORY:
 *
*******************************************************************************/
static CLI_CMD_NODE_T *cli2_core_create_cmd(
	IN OUT I8_T *cmd,
	IN UI8_T level,
	OUT CLI_CMD_NODE_T **path,
	OUT UI32_T *path_num)
{
	CLI_CMD_NODE_T *path_node;
	CLI_CORE_WORD_T word;
	UI32_T num;
	BOOL_T is_new;

	num = *path_num;
	/* from root, create or search the path */
	path_node = path[num - 1];

	do
	{
		if (cmd == NULL || *cmd == '\0'/*NULL*/)
		{
			/* first time checking for the command start with parameter "/<int> xxx" */
			memset(&word, 0, sizeof(word));
		}
		else
		{
			cmd = cli2_core_get_syncmd_word(cmd, &word);
			if (!cmd)
			{
				return NULL;
			}
		}

#if 0
		/* A 'cmd <string>' with a 'cmd aaa bbb' have indeterminacy, so can't enable register command like this case
		 * for create command, since the parse is need, then should check after parameter parse(register)
		 */

		/* for create path, check the command existed and it follow a string parameter */
		path_node = CLI_DB_Find_Entry(path_node, CLI_TYPE_CMD, word.name, word.name_len, FALSE, NULL);
		if (path_node && path_node->callback && (cli2_core_check_first_parameter_is_string(path_node)))
		{
			/* error: indeterminacy path with command! */
			cli2_core_destory_path(path, num, NULL);
			return NULL;
		}
#else
		/* find/create path */
		path_node = CLI_DB_Find_Entry(path_node, CLI_TYPE_CMD, word.name, word.name_len, TRUE, &is_new);
		if (!path_node)
		{
			/* memory allocated fail! */
			cli2_core_destory_path(path, num, NULL);
			return NULL;
		}
#endif

		if (is_new)
		{
			path_node->para = NULL;
			path_node->callback = NULL;
			if (word.help_en_len)
			{
				path_node->help_en = CLI_MISC_Malloc(word.help_en_len + word.help_local_len + 2);
				strncpy(path_node->help_en, word.help_en, word.help_en_len);
				path_node->help_en[word.help_en_len] = '\0';
				if (word.help_local_len)
				{
					path_node->help_local = path_node->help_en + word.help_en_len + 1;
					strncpy(path_node->help_local, word.help_local, word.help_local_len);
					path_node->help_local[word.help_local_len] = '\0';
				}
				else
				{
					path_node->help_local = path_node->help_en;
				}
				path_node->help_alloc_flag = TRUE;
			}
			else
			{
				path_node->help_en = NULL;
				path_node->help_local = NULL;
				path_node->help_alloc_flag = FALSE;
			}
		}
		else
		{
			if (word.help_en_len && (path_node->help_en == NULL))
			{
				path_node->help_en = CLI_MISC_Malloc(word.help_en_len + word.help_local_len + 2);
				strncpy(path_node->help_en, word.help_en, word.help_en_len);
				path_node->help_en[word.help_en_len] = '\0';
				if (word.help_local_len)
				{
					path_node->help_local = path_node->help_en + word.help_en_len + 1;
					strncpy(path_node->help_local, word.help_local, word.help_local_len);
					path_node->help_local[word.help_local_len] = '\0';
				}
				else
				{
					path_node->help_local = path_node->help_en;
				}
				path_node->help_alloc_flag = TRUE;
			}
		}

		/* set the access level */
		if ((is_new) || (path_node->level > level))
		{
			/* new path or old level > this level, set this path to the min level */
			path_node->level = level;
		}

		path[num++] = path_node;
	}while (cmd && *cmd);

	/* a same command already exist ! */
	if (path_node->callback) 
	{
		return NULL;
	}

	/* set command node access level */
	path_node->cmd_level = level;
	path_node->register_cmd_level = level;

	/* update output path number */
	*path_num = num;

	/* return the command node */
	return path_node;
}



/*******************************************************************************
 *
 * FUNCTION NAME:
 *   cli2_core_deregister_parameter
 *
 * DESCRIPTION:
 *   free the command parameter
 *
 * INPUT:
 *   cmd_node	: the pointer of command node
 *
 * OUTPUT:
 *   cmd_node	: updated command node
 *
 * RETURN:
 *   None
 *
 * NOTES:
 *   None
 *
 * HISTORY:
 *
*******************************************************************************/
static void cli2_core_deregister_parameter(IN OUT CLI_CMD_NODE_T *cmd_node)
{
	cli_misc_free(cmd_node->para);
	cmd_node->para = NULL;
}



/*******************************************************************************
 *
 * FUNCTION NAME:
 *   cli_core_reg_cmd
 *
 * DESCRIPTION:
 *   register a command:
 *     1. create the command node and get the begin position of parameter part
 *     2. register the command's parameter part
 *
 * INPUT:
 *   vision       : the version v1 or v2
 *   cmd_desc     : the command string
 *   callbackFunc : the command's callback function
 *   flag         : the access level and flag of command (low 8bits is the level)
 *   help_en      : english help message
 *   help_local   : second language help message
 *   user_data    : user data as a cookie
 *
 * OUTPUT:
 *   None
 *
 * RETURN:
 *   TRUE : success
 *   FALSE: error when create command:
 *     empty-string or invalid-word or memory-alloc-fail or command-existed
 *     error when register parameter:
 *
 * NOTES:
 *   None
 *
 * HISTORY:
 *
*******************************************************************************/
BOOL_T cli_core_reg_cmd
	(
	I8_T 			*cmd_desc,
	CLI_CMD_FUNC_T callbackFunc,
	I32_T 			flag,
	I8_T 			*help_en,
	I8_T 			*help_local,
	void 			*user_data
	)
{
	CLI_DBG_ENTER(l_mdl_id);

	I8_T 			*view;
	I8_T 			*cmd;
	I8_T 			*para;
	CLI_CMD_NODE_T *cmd_node;
	CLI_CMD_NODE_T *path_node[CLI_MAX_CMD_WORD];
	UI32_T 			path_num;
	UI8_T 			level;

	if (FALSE == CLI_PARSE_Preprocess_Syncmd(cmd_desc, &view, &cmd, &para))
	{
		xprintf("\r\nInvalidCmd(word error):---%s---\r\n", cmd_desc);
		CLI_DBG_LEAVE(l_mdl_id);
		return FALSE;
	}

	level = (UI8_T) (flag & 0xFF);

	/* register a view */
	if (cli2_core_create_view(view, level, path_node, &path_num) == FALSE)
	{
		xprintf("\r\nInvalidCmd(view error):---%s---\r\n", cmd_desc);
		CLI_DBG_LEAVE(l_mdl_id);
		return FALSE;
	}

	/* register the command node */
	cmd_node = cli2_core_create_cmd(cmd, level, path_node, &path_num);
	if (cmd_node == NULL)
	{
		xprintf("\n\rInvalidCmd(command error):---%s---\r\n", cmd_desc);
		CLI_DBG_LEAVE(l_mdl_id);
		return FALSE;
	}

	/* parse the command parameter */
	cmd_node->para = NULL;
	if (para)
	{
		if (CLI_PARA_Parse_SyncmdPara(para, &cmd_node->para) == FALSE)
		{
			xprintf("\r\nInvalidCmd(parameter error):---%s---\r\n", cmd_desc);
			xprintf("\r\nparameter error:---%s---\r\n", para);

			/* already include cmd_node in path_node[] */
			cli2_core_destory_path(path_node, path_num, NULL);
			CLI_DBG_LEAVE(l_mdl_id);
			return FALSE;
		}

		/* A 'cmd <string>' with a 'cmd aaa bbb' have ambiguous, so can't enable register command like this case */
		if ((cli2_core_check_first_parameter_is_string(cmd_node)) && (cli2_core_check_same_name_path_exist(path_node[path_num - 1], cmd_node)))
		{
			cli2_core_deregister_parameter(cmd_node);
			xprintf("\r\nInvalidCmd(ambiguous):---%s---\r\n", cmd_desc);

			/* already include cmd_node in path_node[] */
			cli2_core_destory_path(path_node, path_num, NULL);
			CLI_DBG_LEAVE(l_mdl_id);
			return FALSE;
		}
	}

	CLI_DBG_TAG(l_mdl_id);
	/* save the command info. */
	//cmd_node->version = version;
	cmd_node->callback = callbackFunc;
	cmd_node->cookie = user_data;

	if (!cmd_node->help_en)
	{
		cmd_node->help_en = help_en;
		cmd_node->help_local = help_local;
		cmd_node->help_alloc_flag = FALSE;
	}

	CLI_DBG_LEAVE(l_mdl_id);
	return TRUE;
}



/*******************************************************************************
 *
 * FUNCTION NAME:
 *   cli2_core_find_cmd
 *
 * DESCRIPTION:
 *   Find a command node in db.
 *
 * INPUT:
 *   cmd_desc : registered command (not include parameter part)
 *
 * OUTPUT:
 *   path     : result path nodes.
 *   path_num : path nodes already be created or found.
 *
 * RETURN:
 *   pointer  : the found pointer
 *   NULL	  : not found
 *
 * NOTES:
 *   None
 *
 * HISTORY:
 *
 *
*******************************************************************************/
static CLI_CMD_NODE_T *cli2_core_find_cmd(
	IN const I8_T *cmd_desc,
	OUT CLI_CMD_NODE_T **path,
	OUT UI32_T *path_num)
{
	I8_T *view;
	I8_T *cmd;
	I8_T *para;
	BOOL_T is_new;
	I8_T *name;
	I32_T name_len;
	CLI_CMD_NODE_T *path_node;
	CLI_CORE_WORD_T word;
	UI32_T num = 0;

	if (cmd_desc == NULL)
	{
		return NULL;
	}

	if (CLI_PARSE_Preprocess_Syncmd (cmd_desc, &view, &cmd, &para) == FALSE)
	{
		xprintf("InvalidCmd(word error):---%s---\r\n", cmd_desc);
		return NULL;
	}

	path_node = CLI_ROOT_PATH;
	path[num++] = CLI_ROOT_PATH;

	if (view)
	{
		/* search view */
		name = view;
		while (1)
		{
			if ((*view == '/') || (*view == '\0'))
			{
				name_len = view - name;
				path_node = CLI_DB_Find_Entry(path_node, CLI_TYPE_VIEW, name,
					name_len, FALSE, &is_new);
				if (path_node == NULL)
				{
					return NULL;
				}

				path[num++] = path_node;
				name = view + 1;
			}

			if (*view == '\0')
			{
				break;
			}

			view++;
		}
	}

	/* search the command node from the got view tree */
	do
	{
		if (cmd == NULL || *cmd == '\0'/*NULL*/)
		{
			/* first time checking for the command start with parameter "/<int> xxx" */
			memset(&word, 0, sizeof(word));
		}
		else
		{
			cmd = cli2_core_get_syncmd_word(cmd, &word);
			if (!cmd)
			{
				return NULL;
			}
		}

		/* find node */
		path_node = CLI_DB_Find_Entry(path_node,
			CLI_TYPE_CMD, word.name, word.name_len, FALSE, &is_new);
		if (!path_node)
		{
			return NULL;
		}

		path[num++] = path_node;
	}while (cmd && *cmd);

	/* update output path number */
	*path_num = num;

	/* return the command node */
	return path_node;
}



/*************************************************
FUNCTION NAME:
	cli_core_dereg_cmd

DESCRIPTION:
	deregister a command:
		1. find the command and its path
		2. free the parameter and information
		3. destory the command and its path (update the path's level also)

INPUT:
	cmd_desc	: the pointer of command string pointer

OUTPUT:

RETURN:
	TRUE		: success
	FALSE		: command not exist

NOTES:

HISTORY:

*************************************************/
BOOL_T cli_core_dereg_cmd(I8_T * cmd_desc)
{
	CLI_CMD_NODE_T *cmd_node;
	CLI_CMD_NODE_T *path_node[CLI_MAX_CMD_WORD];
	UI32_T path_num;

	/* find the path nodes and command node */
	cmd_node = cli2_core_find_cmd(cmd_desc, path_node, &path_num);
	if (cmd_node == NULL)
	{
		/* can't found the command */
		return FALSE;
	}

	/* free the parameter */
	cli2_core_deregister_parameter(cmd_node);

	/* free command info. */
	cmd_node->callback = NULL;
	cmd_node->cmd_level = 0;
	cmd_node->register_cmd_level = 0;

	/* destory the path (include the command node) */
	cli2_core_destory_path(path_node, path_num, NULL);

	/* command deregister success */
	return TRUE;
}



/*******************************************************************************
 *
 * FUNCTION NAME:
 *   __parse_usrcmd
 *
 * DESCRIPTION:
 *   Parse a user command to wordlist.(each word is seperated by white spaces. or a pair of ")
 *
 * INPUT:
 *   usrcmd: user command (terminated by NULL or ';')
 *
 * OUTPUT:
 *   wordlist: result word list
 *
 * RETURN:
 *   None
 *
 * NOTES:
 *   None
 *
 * HISTORY:
 *   2009-Aug-10 by wangbin -- Create
 *
*******************************************************************************/
static void __parse_usrcmd(
	IN const I8_T *usrcmd,
	OUT CLI_PRIV_USRCMD_WORDLIST_T *wordlist)
{
	const I8_T *temp_ptr;

	wordlist->count = 0;
	wordlist->index = 0;
	wordlist->usrcmd = usrcmd;

	while (*usrcmd)
	{
		if (*usrcmd == '"' && (*(usrcmd - 1) == ' ' || *(usrcmd - 1) == '\t'))
		{
			temp_ptr = strchr(usrcmd + 1, '"');
			if (temp_ptr && (*(temp_ptr + 1) == ' ' || *(temp_ptr + 1) == '\t' || *(temp_ptr + 1) == '\0'))
			{
				usrcmd++;
				wordlist->len[wordlist->count] = 0;
				wordlist->word[wordlist->count] = usrcmd;
				while ((*usrcmd != 0) && (*usrcmd != '"'))
				{
					wordlist->len[wordlist->count]++;
					usrcmd++;
				}
				if (*usrcmd == '"')
				{   /* skip the end of the '"' */
					usrcmd++;
				}
				wordlist->count++;
				continue;
			}
		}

		if ((*usrcmd == ' ') || (*usrcmd == '\t'))
		{
			usrcmd++;
		}
		else
		{
			wordlist->len[wordlist->count] = 0;
			wordlist->word[wordlist->count] = usrcmd;
			while ((*usrcmd != '\0') && (*usrcmd != ' ') && (*usrcmd != '\t'))
			{
				wordlist->len[wordlist->count]++;
				usrcmd++;
			}

			wordlist->count++;
			if (wordlist->count >= CLI_PRIV_MAX_USRCMD_WORD)
			{
				xprintf("\r\nToo many words in command.\r\n");
				break;
			}
		}
	}

	wordlist->word[wordlist->count] = usrcmd;
	wordlist->len[wordlist->count] = 0;
	wordlist->count++;

	return;
}



/*******************************************************************************
 *
 * FUNCTION NAME:
 *   cli2_core_get_next_near_node
 *
 * DESCRIPTION:
 *   get the next node that match the given name+len+level+type
 *
 * INPUT:
 *   path: the path
 *   node: the prev node or NULL for get first node
 *   name: the given name
 *   len: the name length
 *   level: the acess level
 *   type_bits: type bitmap for match one or more type
 *
 * OUTPUT:
 *   None
 *
 * RETURN:
 *   pointer: the found node
 *   NULL: no node found
 *
 * NOTES:
 *   None
 *
 * HISTORY:
 *
*******************************************************************************/
static CLI_CMD_NODE_T *cli2_core_get_next_near_node(
	IN CLI_CMD_NODE_T *path,
	IN CLI_CMD_NODE_T *node,
	IN const I8_T *name,
	IN UI32_T len,
	IN UI8_T level,
	IN UI8_T type_bits)
{
	/* get next node or first node that >= name+len */
	if (node)
	{
		node = CLI_DB_Get_NextEntry(path, node);
	}
	else
	{
		/* get first node that >= name+len */
		node = CLI_DB_Get_NearEntryByName(path, name, len, type_bits);
	}

	while (node)
	{
		/* get the node name and check node name have same prefix with given name+len */
		if (strncmp(node->name, name, len) != 0)
		{
			/* name prefix not match */
			node = NULL;
			break;
		}

		/* check level and type */
		if ((level >= node->level) && (node->type & type_bits))
		{
			break;
		}

		/* check next node */
		node = CLI_DB_Get_NextEntry(path, node);
	}

	return node;
}

/*******************************************************************************
 *
 * FUNCTION NAME:
 *   cli2_core_match_cmd
 *
 * DESCRIPTION:
 *   search the command with given name+len and level
 *
 * INPUT:
 *   path: the path
 *   name: the given ame
 *   len: the name length
 *   level: the acess level
 *
 * OUTPUT:
 *   None
 *
 * RETURN:
 *   pointer: the found view node
 *   NULL: no view matched
 *
 * NOTES:
 *   None
 *
 * HISTORY:
 *
*******************************************************************************/
static CLI_CMD_NODE_T *cli2_core_match_cmd(
	IN CLI_CMD_NODE_T *path,
	IN const I8_T *word,
	IN UI32_T len,
	IN UI8_T level)
{
	CLI_CMD_NODE_T *node;
	CLI_CMD_NODE_T *next_node;

	/* get the first node that >= given word+len */
	node = cli2_core_get_next_near_node(path, NULL, word, len, level, CLI_TYPE_CMD);
	if (node == NULL)
	{
		/* not match */
		return NULL;
	}

	/* get the node word and check full match */
	if (node->name[len] == '\0')
	{
		/* full match */
		return node;
	}

	/* not full match */
	next_node = cli2_core_get_next_near_node(path, node, word, len, level, CLI_TYPE_CMD);
	if (next_node == NULL)
	{
		/* no other node match, we got the node */
		return node;
	}
	else
	{
		return CLI_AMBIGUOUS_NODE;
	}
}

/*******************************************************************************
 *
 * FUNCTION NAME:
 *   cli2_core_match_usrcmd_word
 *
 * DESCRIPTION:
 *   Get last of the command name node.
 *
 * INPUT:
 *   level: user's current level
 *   max_index: how many word to be checked
 *   plist: word list
 *   p_node: current view's node
 *
 * OUTPUT:
 *   error_code: error code
 *
 * RETURN:
 *   //###
 *
 * NOTES:
 *   None
 *
 * HISTORY:
 *   2010-Jan-13 by wangbin -- Create
 *
*******************************************************************************/
static CLI_CMD_NODE_T *cli2_core_match_usrcmd_word(
	IN CLI_CMD_NODE_T *p_node,
	IN CLI_PRIV_USRCMD_WORDLIST_T *plist,
	OUT I32_T *error_code,
	IN I32_T max_index,
	IN UI8_T level)
{
	CLI_CMD_NODE_T *cur_node;
	CLI_CMD_NODE_T *save_node;
	CLI_CMD_NODE_T *path_node;
	I32_T i;
	CLI_PRIV_USRCMD_WORDLIST_T temp_word_list;
	cli_value_t *cli2_value;
	I32_T error_position;
	BOOL_T para_match;

	/* from root, search the path */
	*error_code = 0;
	path_node = p_node;
	cur_node = NULL;
	save_node = NULL;
	cli2_value = NULL;
	para_match = FALSE;

	/* check this function use the command download in CLIG1 */
	for (i = 0; i < max_index; i++)
	{
		plist->index = i;
		cur_node = cli2_core_match_cmd(path_node, plist->word[i], plist->len[i], level);
		if ((cur_node == NULL) || (cur_node == CLI_AMBIGUOUS_NODE))
		{
			break;
		}

		/* check if the word is ambiguous */
		if (para_match)
		{
			/* the word matches parameters of previous node command and matched current node */
			/* clean the matched flag of previous parameter */
			para_match = FALSE;

			/* if the current word is not best match, will determine it is ambiguous */
			if (cur_node->name[plist->len[i]] != '\0')
			{
				cur_node = CLI_AMBIGUOUS_NODE;
				break;
			}
			/* if the current word is matched best, choose current node.*/
		}

		/* check if the after words match current node's parameters */
		if ((i + 1 < max_index) && (cur_node->callback) && (level >= cur_node->cmd_level))
		{
			memcpy(&temp_word_list, plist, sizeof(CLI_PRIV_USRCMD_WORDLIST_T));
			/* check begin with next word */
			temp_word_list.index = i + 1;
			CLI_PARA_Get_Value(&temp_word_list, cur_node->para, &cli2_value, &error_position);
			if (cli2_value)
			{
				CLI_PARA_Free_ValueList(cli2_value);
				/* don't free again for next command node (if the parameter parsed don't matched) */
				cli2_value = NULL;
			}

			/* have word matched the parameters */
			if (temp_word_list.index > i + 1)
			{
				para_match = TRUE;
			}
		}

		save_node = cur_node;
		path_node = cur_node;
	}

	if (cur_node)
	{
		if (cur_node == CLI_AMBIGUOUS_NODE)
		{
			*error_code = CLI_PRIV_AMBIGUOUS; /* ambiguous node */
			return NULL;
		}
		plist->index = i;
		return cur_node;
	}
	else
	{
		return save_node;
	}
}

/*******************************************************************************
 *
 * FUNCTION NAME:
 *   CLI_CORE_GetView
 *
 * DESCRIPTION:
 *   Get data of view
 *
 * INPUT:
 *   dir: pointer of the directory
 *   level: access level of the current user
 *
 * OUTPUT:
 *   None
 *
 * RETURN:
 *   pointer of the dir data
 *
 * NOTES:
 *   None
 *
 * HISTORY:
 *   2009-Aug-05 by wangbin -- Create
 *
*******************************************************************************/
CLI_CMD_NODE_T *CLI_CORE_GetView (IN I8_T *dir, IN UI8_T level)
{
	CLI_CMD_NODE_T *path_node; /* node of the path */
	CLI_CMD_NODE_T *node = NULL;

	const I8_T *name;
	const I8_T *view;
	I32_T name_len;

	view = dir;
	path_node = CLI_ROOT_PATH;
	while ((*view == ' ') || (*view == '\t') || (*view == '/'))
	{
		view++;
	}

	if (*view == '\0')
	{
		return path_node;
	}

	while (*view)
	{
		while ((*view == ' ') || (*view == '\t') || (*view == '/'))
		{
			view++;
		}
		name = view;
		while (*view && (*view != '/'))
		{
			view++;
		}
		name_len = view - name;
		if (name_len)
		{
			/* get the first node that >= given name+len+level+type */
			node = cli2_core_get_next_near_node(path_node, CLI_ROOT_PATH,
				name, name_len, level, CLI_TYPE_VIEW);
			if (node == NULL)
			{
				/* not exist this path */
				return (CLI_CMD_NODE_T *)CLI_NULL_PATH;
			}
			/* check full name match */
			if (node->name[name_len] == '\0')
			{
				/* full match (since sorted by name, so full match node must be the first) */
				path_node = node;
				continue;
			}

			/* check ambiguous */
			if (cli2_core_get_next_near_node(path_node, node,
				name, name_len, level, CLI_TYPE_VIEW))
			{
				/* more than one name match the given view name */
				/* should set this path not exist */
				return (CLI_CMD_NODE_T *)CLI_NULL_PATH;
			}
			path_node = node;
		}
	}

	return node;
}

/*******************************************************************************
 *
 * FUNCTION NAME:
 *   cli2_core_parse_cmd_in_view
 *
 * DESCRIPTION:
 *   parse a command line in a view
 *
 * INPUT:
 *   level: user's access level
 *   flag: the access level and flag of command (low 8bits is the level)
 *   helpMsg: english help message
 *   auxhelpMsg: second language help message
 *   user_data: user data as a cookie
 *
 * OUTPUT:
 *   para_list: the parameter list
 *   error_position: error position of command line
 *
 * RETURN:
 *   pointer: found command node
 *   NULL: match command fail or parse parameter fail
 *
 * NOTES:
 *   None
 *
 * HISTORY:
 *
*******************************************************************************/
CLI_CMD_NODE_T *cli2_core_parse_cmd_in_view(
	IN I8_T *view,
	CLI_PRIV_USRCMD_WORDLIST_T *plist,
	IN UI8_T level,
	OUT cli_value_t **para_list,
	OUT I32_T *error_position)
{
	CLI_DBG_ENTER(l_mdl_id);

	CLI_CMD_NODE_T *cmd_node;
	CLI_CMD_NODE_T *path_node;
	I32_T error_code;
	UI32_T para_err_pos = 0;

	*error_position = 0;

	path_node = CLI_CORE_GetView(view, level);
	if (path_node == (CLI_CMD_NODE_T *)CLI_NULL_PATH)
	{
		CLI_DBG_LEAVE(l_mdl_id);
		return NULL;
	}

	cmd_node = cli2_core_match_usrcmd_word(path_node, plist, &error_code, plist->count - 1, level);
	if (!cmd_node)
	{
		/* get the command started with parameter "/<int> xxx" */
		cmd_node = CLI_DB_Find_Entry(path_node, CLI_TYPE_CMD, NULL, 0, FALSE, NULL);
		if (cmd_node && level < cmd_node->cmd_level)
		{
			/* check the level */
			cmd_node = NULL;
		}
	}

	if (!cmd_node || !cmd_node->callback || level < cmd_node->cmd_level)
	{
		*error_position = plist->word[plist->index] - plist->usrcmd;
		if (error_code == CLI_PRIV_AMBIGUOUS)
		{
			CLI_DBG_LEAVE(l_mdl_id);
			return CLI_AMBIGUOUS_NODE;
		}

		CLI_DBG_LEAVE(l_mdl_id);
		return NULL;
	}

	if (CLI_PARA_Get_Value(plist, cmd_node->para, para_list, (I32_T *)&para_err_pos) >= 0)
	{
		*error_position = para_err_pos;
		CLI_DBG_LEAVE(l_mdl_id);
		return cmd_node;
	}
	else
	{
		*error_position = para_err_pos;
		CLI_DBG_LEAVE(l_mdl_id);
		return NULL;
	}
}



/*******************************************************************************
 *
 * FUNCTION NAME:
 *   CLI_CORE_Parse_Cmdstr
 *
 * DESCRIPTION:
 *   parse the command and parameter -- 
 *   1. Parse a user command to wordlist
 *   2. Parse command in current view
 *   3. Parse command in global view
 *   4. Return found command node
 *
 * INPUT:
 *   view      : current view the command executed.
 *   usrcmd    : the command line
 *   level     : user's access level
 *   flag      : the access level and flag of command (low 8bits is the level)
 *   helpMsg   : english help message
 *   auxhelpMsg: second language help message
 *   user_data : user data as a cookie
 *
 * OUTPUT:
 *   para_list     : the parameter list
 *   error_position: error position of command line
 *
 * RETURN:
 *   pointer: found command node
 *   NULL   : match command fail or parse parameter fail
 *
 * NOTES:
 *   None
 *
 * HISTORY:
 *
*******************************************************************************/
CLI_CMD_NODE_T *CLI_CORE_Parse_Cmdstr(
	IN I8_T *view,
	IN I8_T *usrcmd,
	IN UI8_T level,
	OUT cli_value_t **para_list,
	OUT I32_T *error_position)
{
	CLI_CMD_NODE_T *cmd_node;
	CLI_PRIV_USRCMD_WORDLIST_T *plist;
	I32_T save_pos;

	/* parse command to wordlist */
	plist = CLI_MISC_Malloc(sizeof(CLI_PRIV_USRCMD_WORDLIST_T));
	__parse_usrcmd(usrcmd, plist);
	if (plist->count == 0)
	{
		cli_misc_free(plist);
		return NULL;
	}

	/* search command in current view */
	*error_position = 0;
	cmd_node = cli2_core_parse_cmd_in_view(view, plist, level, para_list, error_position);
	CLI_DBG_CPL(l_mdl_id, "error_position=%d,view=%s\r\n", *error_position, view);

	/* if command is'not in current view, then search command in global view */
	if (!cmd_node)
	{
		save_pos = 0;
		cmd_node = cli2_core_parse_cmd_in_view(CLI_MOD_GLOBAL_PATH, plist, level, para_list, &save_pos);
		if (!cmd_node)
		{
			if (save_pos > *error_position)
			{
				*error_position = save_pos;
			}
			CLI_DBG_CPL(l_mdl_id, "error_position=%d,view=%s\r\n", *error_position, CLI_MOD_GLOBAL_PATH);
		}
	}

	cli_misc_free(plist);
	return cmd_node;
}



/*******************************************************************************
 *
 * FUNCTION NAME:
 *	 cli2_core_get_helpList
 *
 * DESCRIPTION:
 *	 Get all possible word in path_node
 *
 * INPUT:
 *	 helplist  -- pointer of the result wordlist
 *	 path_node -- dir nodes to be checked
 *	 cmd_str   -- user command to be checked
 *	 level     -- the level of the user who input <cmd_str>
 *
 * OUTPUT:
 *	 helplist  -- result word list
 *
 * RETURN:
 *	 < 0 -- failure
 *	 = 0 -- success
 *
 * NOTES:
 *	 NA
 *
 * HISTORY:
 *	 2009-Mar-27 by wangbin -- Create
 *
*******************************************************************************/
static I32_T cli2_core_get_helpList(
	IN OUT CLI_PRIV_HELPLIST_T *helplist,
	IN I8_T *view,
	IN CLI_PRIV_USRCMD_WORDLIST_T *plist,
	IN UI8_T level)
{
	CLI_DBG_CPL(l_mdl_id, "helplist->local_help=%d\r\n", helplist->local_help);

	CLI_CMD_NODE_T *cur_node;
	CLI_CMD_NODE_T *path_node;
	I32_T error_code;

	CLI_CMD_NODE_T *node;

	I32_T help_name_len;
	I32_T ret_value;
	I32_T previous_max;

	if (helplist->nexthelp)
	{
		previous_max = plist->count - 1;
	}
	else
	{
		previous_max = plist->count - 2;
	}

	path_node = CLI_CORE_GetView(view, level);

	if (path_node == (CLI_CMD_NODE_T *)CLI_NULL_PATH)
	{
		return -1;
	}

	if (previous_max > 0)
	{
		cur_node = cli2_core_match_usrcmd_word(path_node, plist, &error_code, previous_max, level);
		/* update the parsed process */
		helplist->error_pos = plist->word[plist->index] - plist->usrcmd;

		if (!cur_node)
		{
			/* get the command started with parameter "/<int> xxx" */
			cur_node = CLI_DB_Find_Entry(path_node, CLI_TYPE_CMD, NULL, 0, FALSE, NULL);
			if (cur_node && level < cur_node->cmd_level)
			{
				/* check the level */
				cur_node = NULL;
			}
		}

		if (!cur_node && previous_max)
		{
			if (error_code == CLI_PRIV_AMBIGUOUS)
			{
				return -2;
			}

			return -1;
		}
		path_node= cur_node;
	}
	else
	{
		/* get the command started with parameter "/<int> xxx" */
		cur_node = CLI_DB_Find_Entry(path_node, CLI_TYPE_CMD, NULL, 0, FALSE, NULL);
		if (cur_node && level < cur_node->cmd_level)
		{
			/* check the level */
			cur_node = NULL;
		}
		plist->index = 0;
	}

	if (plist->index == previous_max)
	{
		node = NULL;
		while ((node = cli2_core_get_next_near_node(path_node, node, plist->word[plist->index],
			plist->len[plist->index], level, CLI_TYPE_CMD)) != NULL)
		{
			if (strlen(node->name) == 0)
			{
				/* the command started with parameter "/<int> xxx"
				  * so the command word is null, nothing for input, continue. */
				continue;
			}

			// get node success
			helplist->name[helplist->count] = helplist->name_buf + helplist->name_buf_used;
			strcpy(helplist->name[helplist->count], node->name);
			CLI_DBG_CPL(l_mdl_id, "helplist->local_help=%d,node->help_local=0x%08X,%s\r\n",
								  helplist->local_help, (int)node->help_local, node->help_local);
			if (helplist->local_help && node->help_local && node->help_local[0])
			{
				CLI_DBG_TAG(l_mdl_id);
				helplist->help[helplist->count] = node->help_local;
			}
			else
			{
				if (node->help_en && node->help_en[0])
				{
					CLI_DBG_TAG(l_mdl_id);
					helplist->help[helplist->count] = node->help_en;
				}
				else
				{
					CLI_DBG_TAG(l_mdl_id);
					helplist->help[helplist->count] = NULL;
				}
			}

			// get the max length of the name
			help_name_len = strlen (helplist->name[helplist->count]);
			helplist->name_buf_used += help_name_len + 1;
			if (helplist->max_name_len < help_name_len)
			{
				helplist->max_name_len = help_name_len;
			}
			helplist->count++; // increase the counter
		}
	}

	if (cur_node && cur_node->callback && level >= cur_node->cmd_level)
	{
		/* get para names which match the next word */
		if (helplist->nexthelp)
		{
			ret_value = CLI_PARA_Get_Help (plist, cur_node->para, helplist, CLI_PARA_GET_NEXT_WORDLIST);
		}
		else
		{
			ret_value = CLI_PARA_Get_Help (plist, cur_node->para, helplist, CLI_PARA_GET_CUR_WORDLIST);
		}

		CLI_DBG_CPL(l_mdl_id, "ret_value=%u\r\n", ret_value);
	}

	/* return the found node */
	if (helplist->count <= 0 || plist->index < previous_max)
	{
		return -1;
	}
	else
	{
		return 0;
	}
}



/**************************************************************************
FUNCTION NAME:
	cli2_core_merge_helpList

DESCRIPTION:
	Merge the input help lists.

INPUT:
	org_helpList	: origin help list
	src_helpList	: the help list which need to be merged

OUTPUT:
	org_helpList	: the help list after merged

RETURN:
	None

NOTES:
	input helplist format should be: <parameter> -> keyword -> <cr>

HISTORY:

**************************************************************************/
void cli2_core_merge_helpList (
	IN OUT CLI_PRIV_HELPLIST_T *org_helpList,
	IN CLI_PRIV_HELPLIST_T *src_helpList)
{
	I32_T org_cnt, src_cnt;
	I32_T i, j, cmp_value;
	I8_T *temp_name;
	const I8_T *temp_help;
	BOOL_T end_flag;
	CLI_PRIV_HELPLIST_T *dst_helpList;

	/* the input helplist format should be: <parameter> -> keyword -> <cr> and merge together */
	end_flag = FALSE;
	/* find the <cr>, not process in loop */
	if (org_helpList->count > 0)
	{
		if (strcmp(org_helpList->name[org_helpList->count - 1], "<cr>") == 0)
		{
			org_cnt = org_helpList->count - 1;
			end_flag = TRUE;
		}
		else
		{
			org_cnt = org_helpList->count;
		}
	}
	else
	{
		org_cnt = 0;
	}
	/* find the <cr>, not process in loop */
	if (src_helpList->count > 0)
	{
		if (strcmp(src_helpList->name[src_helpList->count - 1], "<cr>") == 0)
		{
			src_cnt = src_helpList->count - 1;
			end_flag = TRUE;
		}
		else
		{
			src_cnt = src_helpList->count;
		}
	}
	else
	{
		src_cnt = 0;
	}

	/* init the destination helplist */
	dst_helpList = CLI_MISC_Malloc(sizeof(CLI_PRIV_HELPLIST_T));
	memset(dst_helpList, 0, sizeof(CLI_PRIV_HELPLIST_T));
	dst_helpList->error_pos = org_helpList->error_pos;
	dst_helpList->local_help = org_helpList->local_help;
	dst_helpList->nexthelp = org_helpList->nexthelp;
	dst_helpList->max_name_len =
		(org_helpList->max_name_len > src_helpList->max_name_len)?
		org_helpList->max_name_len : src_helpList->max_name_len;

	i = 0;
	j = 0;
	while (i < org_cnt || j < src_cnt)
	{
		/* find out the smaller one and merge in dst_helpList */
		if (i < org_cnt && j < src_cnt)
		{
			cmp_value = strcmp(org_helpList->name[i], src_helpList->name[j]);
			if (cmp_value < 0)
			{
				temp_name = org_helpList->name[i];
				temp_help = org_helpList->help[i];
				i++;
			}
			else if (cmp_value == 0)
			{
				/* if equal, just merge one */
				temp_name = org_helpList->name[i];
				temp_help = org_helpList->help[i];
				i++;
				j++;
			}
			else
			{
				temp_name = src_helpList->name[j];
				temp_help = src_helpList->help[j];
				j++;
			}
		}
		else if (i < org_cnt)
		{
			temp_name = org_helpList->name[i];
			temp_help = org_helpList->help[i];
			i++;
		}
		else
		{
			temp_name = src_helpList->name[j];
			temp_help = src_helpList->help[j];
			j++;
		}

		dst_helpList->name[dst_helpList->count] = dst_helpList->name_buf + dst_helpList->name_buf_used;
		strcpy(dst_helpList->name[dst_helpList->count], temp_name);
		dst_helpList->name_buf_used += strlen(temp_name) + 1;
		dst_helpList->help[dst_helpList->count] = temp_help;
		dst_helpList->count++;
	}

	/* get the number of <parameter> which need to parse */
	for (i = 0; i < dst_helpList->count; i++)
	{
		if (*dst_helpList->name[i] == '<')
		{
			/* find the <parameter> which need to process and increase the para_count */
			dst_helpList->para_count++;
		}
	}

	/* attach the END node */
	if (end_flag)
	{
		dst_helpList->name[dst_helpList->count] = dst_helpList->name_buf + dst_helpList->name_buf_used;
		strcpy(dst_helpList->name[dst_helpList->count], "<cr>");
		dst_helpList->name_buf_used += strlen("<cr>") + 1;
		dst_helpList->help[dst_helpList->count] = NULL;
		dst_helpList->count++; // <cr>
	}

	/* Not just copy. Because after copy the org_helpList->name will point to dst_helpList->name_buf,
	    so the org_helpList->name need change to point to org_helpList->name_buf. */
	memcpy(org_helpList, dst_helpList, sizeof(CLI_PRIV_HELPLIST_T));
	for (i = 0; i < dst_helpList->count; i++)
	{
		/* base address add the offset */
		org_helpList->name[i] = org_helpList->name_buf +
			(dst_helpList->name[i] - dst_helpList->name_buf);
	}

	cli_misc_free(dst_helpList);
}

/**************************************************************************
FUNCTION NAME:
	cli2_core_sequence_helpList

DESCRIPTION:
	Sequence help list.

INPUT:
	dst_helpList	: the help list which need to be sequenced

OUTPUT:
	dst_helpList	: the help list after sequenced

RETURN:
	None

NOTES:
	input help list: keyword -> parameter -> <cr>
	after sequenced: <parameter> -> keyword -> <cr>

HISTORY:

**************************************************************************/
void cli2_core_sequence_helpList (IN OUT CLI_PRIV_HELPLIST_T *dst_helpList)
{
	I32_T i, j, cnt;
	I8_T *temp_name;
	const I8_T *temp_help;

	if (dst_helpList->count < 2)
	{
		/* needn't change the sequence */
		return;
	}

	/* helplist: keyword -> parameter -> <cr> should chang to <parameter> -> keyword -> <cr>*/
	if (strcmp(dst_helpList->name[dst_helpList->count - 1], "<cr>") == 0)
	{
		cnt = dst_helpList->count - 1;
	}
	else
	{
		cnt = dst_helpList->count;
	}

	/* use the chosen algorithm to sequence the list */
	for (i = 0; i < cnt; i++)
	{
		/* the least one will to the begining of the remaining part */
		for (j = i + 1; j < cnt; j++)
		{
			if (strcmp(dst_helpList->name[i], dst_helpList->name[j]) > 0)
			{
				/* change the name */
				temp_name = dst_helpList->name[i];
				dst_helpList->name[i] = dst_helpList->name[j];
				dst_helpList->name[j] = temp_name;

				/* change the according help message */
				temp_help = dst_helpList->help[i];
				dst_helpList->help[i] = dst_helpList->help[j];
				dst_helpList->help[j] = temp_help;
			}
		}
	}
}

/*******************************************************************************
 *
 * FUNCTION NAME:
 *	 CLI_CORE_Get_Help
 *
 * DESCRIPTION:
 *	 Get all possible word of the user cmd in current and global view of default view.
 *
 * INPUT:
 *	 helplist -- pointer of the result wordlist
 *	 view -- current view of the command
 *	 usrcmd -- user command to be checked
 *	 level -- the level of the user who input <cmd_str>
 *
 * OUTPUT:
 *	 helplist -- result word list
 *
 * RETURN:
 *	 < 0 -- failure
 *	 = 0 -- success
 *
 * NOTES:
 *	 NA
 *
 * HISTORY:
 *	 2009-Mar-27 by wangbin -- Create
 *
*******************************************************************************/
I32_T CLI_CORE_Get_Help(
	IN OUT CLI_PRIV_HELPLIST_T *helplist,
	IN I8_T *view,
	IN const I8_T *usrcmd,
	IN UI8_T level)
{
	CLI_DBG_CPL(l_mdl_id, "helplist->local_help=%d\r\n", helplist->local_help);

	CLI_PRIV_USRCMD_WORDLIST_T *plist;
	I32_T ret, ret01, ret02;
	I32_T i;
	CLI_PRIV_HELPLIST_T *temp_globalList;

	ret = 0;
	plist = CLI_MISC_Malloc(sizeof(CLI_PRIV_USRCMD_WORDLIST_T));
	helplist->name_buf_used = 0;
	__parse_usrcmd(usrcmd, plist); /* parse command to wordlist */
	if (plist->count == 0)
	{
		cli_misc_free(plist);
		return ret;
	}

	helplist->count = 0;
	helplist->para_count = 0;
	helplist->error_pos = 0;
	helplist->max_name_len = 0;

	temp_globalList = CLI_MISC_Malloc(sizeof(CLI_PRIV_HELPLIST_T));
	/* now the helplist->name is not used, so just copy is OK */
	memcpy(temp_globalList, helplist, sizeof(CLI_PRIV_HELPLIST_T));

	/* get help list from current view */
	CLI_DBG_CPL(l_mdl_id, "helplist->local_help=%d\r\n", helplist->local_help);
	ret01 = cli2_core_get_helpList(helplist, view, plist, level);
	cli2_core_sequence_helpList(helplist);
	plist->index = 0;

	/* get help list from global view */
	ret02 = cli2_core_get_helpList(temp_globalList, CLI_MOD_GLOBAL_PATH, plist, level);
	cli2_core_sequence_helpList(temp_globalList);

	/* generate the current help list */
	if ((ret01 < 0) && (ret02 < 0))
	{
		if (helplist->error_pos < temp_globalList->error_pos)
		{
			helplist->error_pos = temp_globalList->error_pos;
		}
		ret = -1;
	}
	else if (ret01 < 0)
	{
		/* Not just copy. Because after copy the helplist->name will point to temp_globalList->name_buf,
		    so the helplist->name need change to point to helplist->name_buf. */
		memcpy(helplist, temp_globalList, sizeof(CLI_PRIV_HELPLIST_T));
		for (i = 0; i < temp_globalList->count; i++)
		{
			/* base address add the offset */
			helplist->name[i] = helplist->name_buf + (temp_globalList->name[i] - temp_globalList->name_buf);
		}
	}
	else if (ret02 < 0)
	{
		/* helplist in current view is great, do nothing */
	}
	else
	{
		/* for both success, merge two help information list */
		cli2_core_merge_helpList(helplist, temp_globalList);
	}

	cli_misc_free(temp_globalList);
	cli_misc_free(plist);

	CLI_MISC_Assert(helplist->name_buf_used < sizeof(helplist->name_buf));
	return ret;
}



/*******************************************************************************
 *
 * FUNCTION NAME:
 *   bc_cli_core_init
 *
 * DESCRIPTION:
 *   Init cli2 core
 *
 * INPUT:
 *   None
 *
 * OUTPUT:
 *   None
 *
 * RETURN:
 *   >= 0:
 *
 * NOTES:
 *   TRUE: Init cli2 core success.
 *   FALSE: Init cli2 core failure.
 *
 * HISTORY:
 *   2009-Aug-07 by wangbin -- Create
 *
*******************************************************************************/
BOOL_T bc_cli_core_init (void)
{
	CLI_DB_Init();
	return TRUE;
}

/**************************************************************************
FUNCTION NAME:
	cli2_core_change_childNodeLevel_byPath

DESCRIPTION:
	Change the children node level of the input path node.

INPUT:
	path_node	: path node pointer
	new_level	: new command node level which will be configured

OUTPUT:
	None

RETURN:
	None

NOTES:

HISTORY:
	Creat by wangbin 2011.6.28

**************************************************************************/
static void cli2_core_change_childNodeLevel_byPath(
	IN CLI_CMD_NODE_T *path_node,
	IN I32_T new_level)
{
	CLI_CMD_NODE_T *child;

	/* loop all child under the path */
	child = CLI_DB_Get_FirstEntry(path_node);
	while (child)
	{
		child->level = new_level;
		if (child->callback)
		{
			child->cmd_level = new_level;
		}
		cli2_core_change_childNodeLevel_byPath(child, new_level);

		child = CLI_DB_Get_NextEntry(path_node, child);
	}
}

/**************************************************************************
FUNCTION NAME:
	CLI_CORE_Change_CmdNode_Level

DESCRIPTION:
	Change the command node level.

INPUT:
	cmd_desc	: the pointer of command string
	new_level	: new command node level which will be configured
	all_children	: if all children nodes under the found node will be changed
				  TRUE->the children nodes under the found node will be changed
				  FALSE->just the found command node will be changed
				               (notes: in this case the found node must be a command node,
				               which has callback function.)
OUTPUT:
	None

RETURN:
	TRUE			: change success
	FALSE			: input command string is wrong

NOTES:
	If all_children == FALSE, the input command string must be a complete CLI command,
	(found node has callback function), if not will return FALSE.
	Because just change the path node level is meaningless.

HISTORY:
	Creat by wangbin 2011.6.28

**************************************************************************/
BOOL_T CLI_CORE_Change_CmdNode_Level(
	IN I8_T * cmd_desc,
	IN I32_T new_level,
	IN BOOL_T all_children)
{
	CLI_CMD_NODE_T *cmd_node;
	CLI_CMD_NODE_T *path_node[CLI_MAX_CMD_WORD];
	UI32_T path_num;
	I32_T min_level;
	I32_T i;

	/* find the path nodes and command node */
	cmd_node = cli2_core_find_cmd(cmd_desc, path_node, &path_num);
	if (cmd_node == NULL)
	{
		/* can't found the command */
		return FALSE;
	}

	if (all_children)
	{
		/* change children nodes level */
		cli2_core_change_childNodeLevel_byPath(cmd_node, new_level);
	}
	else
	{
		/* check if input string is a complete command (not include parameter) */
		if (cmd_node->callback == NULL)
		{
			return FALSE;
		}
	}

	/* change current node level */
	if (cmd_node->callback)
	{
		cmd_node->cmd_level = new_level;
	}
	cmd_node->level = new_level;

	/* change all path level */
	for (i = path_num - 1; i > 0; i--)
	{
		/* get the min level of path's childs */
		min_level = cli2_core_get_childs_min_level(path_node[i]);
		if (min_level == -1)
		{
			/* have none child */
			path_node[i]->level = new_level;
		}
		else
		{
			if (path_node[i]->callback)
			{
				if (min_level < path_node[i]->level)
				{
					path_node[i]->level = min_level;
				}
			}
			else
			{
				/* if the node is just path node, it's level is decided by children nodes level */
				path_node[i]->level = min_level;
			}
		}
	}

	/* change level success */
	return TRUE;
}

/**************************************************************************
FUNCTION NAME:
	cli2_core_restore_childNodeLevel_byPath

DESCRIPTION:
	Restore the children node level of the input path node to the original registered level .

INPUT:
	path_node	: path node pointer

OUTPUT:
	None

RETURN:
	None

NOTES:

HISTORY:
	Creat by wangbin 2011.6.28

**************************************************************************/
static void cli2_core_restore_childNodeLevel_byPath(
	IN CLI_CMD_NODE_T *path_node)
{
	CLI_CMD_NODE_T *child;
	I32_T min_level;

	/* loop all child under the path */
	child = CLI_DB_Get_FirstEntry(path_node);
	while (child)
	{
		cli2_core_restore_childNodeLevel_byPath(child);
		if (child->callback)
		{
			child->cmd_level = child->register_cmd_level;
		}
		child->level = child->register_cmd_level;

		min_level = cli2_core_get_childs_min_level(child);
		if (min_level == -1)
		{
			/* have none child */
			child->level = child->register_cmd_level;
		}
		else
		{
			if (child->callback)
			{
				if (min_level < child->register_cmd_level)
				{
					child->level = min_level;
				}
			}
			else
			{
				/* if the node is just path node, it's level is decided by children nodes level */
				child->level = min_level;
			}
		}

		child = CLI_DB_Get_NextEntry(path_node, child);
	}
}

/**************************************************************************
FUNCTION NAME:
	CLI_CORE_Restore_CmdNode_Level

DESCRIPTION:
	Restore the command node level to the original registered level.

INPUT:
	cmd_desc	: the pointer of command string
	all_children	: if all children nodes under the found node will be changed
				  TRUE->the children nodes under the found node will be changed
				  FALSE->just the found command node will be changed
				               (notes: in this case the found node must be a command node,
				               which has callback function.)
OUTPUT:
	None

RETURN:
	TRUE			: change success
	FALSE			: input command string is wrong

NOTES:
	If all_children == FALSE, the input command string must be a complete CLI command,
	(found node has callback function), if not will return FALSE.
	Because just change the path node level is meaningless.

HISTORY:
	Creat by wangbin 2011.6.28

**************************************************************************/
BOOL_T CLI_CORE_Restore_CmdNode_Level(
	IN I8_T * cmd_desc,
	IN BOOL_T all_children)
{
	CLI_CMD_NODE_T *cmd_node;
	CLI_CMD_NODE_T *path_node[CLI_MAX_CMD_WORD];
	UI32_T path_num;
	I32_T min_level;
	I32_T i;

	/* find the path nodes and command node */
	cmd_node = cli2_core_find_cmd(cmd_desc, path_node, &path_num);
	if (cmd_node == NULL)
	{
		/* can't found the command */
		return FALSE;
	}

	if (all_children)
	{
		/* restore children nodes level */
		cli2_core_restore_childNodeLevel_byPath(cmd_node);
	}
	else
	{
		/* check if input string is a complete command (not include parameter) */
		if (cmd_node->callback == NULL)
		{
			return FALSE;
		}
	}

	/* change current node level */
	if (cmd_node->callback)
	{
		cmd_node->cmd_level = cmd_node->register_cmd_level;
	}
	cmd_node->level = cmd_node->register_cmd_level;

	/* change all path level */
	for (i = path_num - 1; i > 0; i--)
	{
		/* get the min level of path's childs */
		min_level = cli2_core_get_childs_min_level(path_node[i]);
		if (min_level == -1)
		{
			/* have none child */
			path_node[i]->level = path_node[i]->register_cmd_level;
		}
		else
		{
			if (path_node[i]->callback)
			{
				if (min_level < path_node[i]->level)
				{
					path_node[i]->level = min_level;
				}
			}
			else
			{
				/* if the node is just path node, it's level is decided by children nodes level */
				path_node[i]->level = min_level;
			}
		}
	}

	/* change level success */
	return TRUE;
}

/* End of file */
