/*
 *  ircd-hybrid: an advanced, lightweight Internet Relay Chat Daemon (ircd)
 *
 *  Copyright (c) 1997-2015 ircd-hybrid development team
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301
 *  USA
 */

/*! \file m_tmode.c
 * \brief Includes required functions for processing the TMODE command.
 * \version $Id: m_tmode.c 5346 2015-01-11 12:41:14Z michael $
 */

#include "stdinc.h"
#include "list.h"
#include "channel.h"
#include "channel_mode.h"
#include "client.h"
#include "hash.h"
#include "ircd.h"
#include "send.h"
#include "parse.h"
#include "modules.h"


/*! \brief TMODE command handler
 *
 * \param source_p Pointer to allocated Client struct from which the message
 *                 originally comes from.  This can be a local or remote client.
 * \param parc     Integer holding the number of supplied arguments.
 * \param parv     Argument vector where parv[0] .. parv[parc-1] are non-NULL
 *                 pointers.
 * \note Valid arguments for this command are:
 *      - parv[0] = command
 *      - parv[1] = timestamp
 *      - parv[2] = channel name
 *      - parv[3] = modes to be added or removed
 */
static int
ms_tmode(struct Client *source_p, int parc, char *parv[])
{
  struct Channel *chptr = NULL;

  if ((chptr = hash_find_channel(parv[2])) == NULL)
  {
    sendto_one_numeric(source_p, &me, ERR_NOSUCHCHANNEL, parv[2]);
    return 0;
  }

  if (atol(parv[1]) > chptr->creationtime)
    return 0;

  if (IsServer(source_p) || HasFlag(source_p, FLAGS_SERVICE))
    set_channel_mode(source_p, chptr, NULL, parc - 3, parv + 3);
  else
  {
    struct Membership *member = find_channel_link(source_p, chptr);

    set_channel_mode(source_p, chptr, member, parc - 3, parv + 3);
  }

  return 0;
}

static struct Message tmode_msgtab =
{
  "TMODE", NULL, 0, 0, 4, MAXPARA, MFLG_SLOW, 0,
  { m_ignore, m_ignore, ms_tmode, m_ignore, m_ignore, m_ignore }
};

static void
module_init(void)
{
  mod_add_cmd(&tmode_msgtab);
}

static void
module_exit(void)
{
  mod_del_cmd(&tmode_msgtab);
}

struct module module_entry =
{
  .node    = { NULL, NULL, NULL },
  .name    = NULL,
  .version = "$Revision: 5346 $",
  .handle  = NULL,
  .modinit = module_init,
  .modexit = module_exit,
  .flags   = MODULE_FLAG_CORE
};
