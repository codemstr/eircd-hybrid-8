/*
 * $Id: cf_database.h,v 1.7 2004/08/24 22:21:52 cfh7 REL_2_1_3 $
 *
 *   OpenChanfix v2.0 
 *
 *   Channel Re-op Service Module for Hybrid 7.0.
 *   Copyright (C) 2003-2004 Thomas Carlsson and Joost Vunderink.
 *   See http://www.garion.org/ocf/ for more information.
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 1, or (at your option)
 *   any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 */




/* Whereas we want to offer as much dynamic control as possible, some memory structures etc. 
 * require static maximums set. Below are static database resource maximums (recompiling
 * after modifying these REQUIRES recreation of the database. Yes, really. Sorry).
 */

/* This defines the maximum channel name length supported by the network */
#define MAXCHANNELNAME (CHANNELLEN + 1)

/* Below value determines how many days the module should keep track of -1, e.g. a value of 15 means we track 14.00-14.99 days */
#define DAYSAMPLES		14

/* This defines the MAXIMUM number of top ops we RETURN in poll queries (default: 10) */
#define OPCOUNT			10

/* This defines the maximum amount of OPS to keep track of per channel (default: 256) */
#define MAXOPCOUNT		256

/* Define the maximum amount of notes allowed per channel */
#define MAXNOTECOUNT 	8

/* Define the maximum length of a note string */
#define MAXNOTESIZE 	550

/* This defines the amount of automatic fixes to retain in history per channel (default: 10) */
#define MAXAUTOFIX	10

/* This defines the amount of manual fixes to retain in history per channel (default: 10) */
#define MAXMANUALFIX	10

/* Initial size of the internal channel cache (default: 256) */
#define DBCACHEINITSIZE 256

/* Expand the internal channel cache by this many elements every time it needs to be enlarged (default: 1024) */
#define DBCACHEINCREASE	1024

/* Maximum amount of channels to support, to thwart resource exhaustion DoS (default: 999999) */
#define DBCACHEMAXCHANS	999999

/* If defined, old channel entries which otherwise would be removed from the database will be retained
 * forever if notes or flags are associated with it (TO-DO: Make dynamic for spring cleaning)
 */
#define REMEMBER_CHANNELS_WITH_NOTES_OR_FLAGS


#define OCF_CHANNEL_FLAG_BLOCK		0x0001
#define OCF_CHANNEL_FLAG_ALERT		0x0002



struct OCF_chanop
{
	char			user[USERLEN + 1];         
	char			host[HOSTLEN + 1];         
	unsigned short		daycount[DAYSAMPLES];
};

struct OCF_chanuser
{
	char			user[USERLEN + 1];         
	char			host[HOSTLEN + 1];         
	int			score;
};

struct OCF_channote
{
	unsigned int		id;
	unsigned int		timestamp;
	char				note[MAXNOTESIZE];
};

struct OCF_channel
{
	char*				name;
	unsigned int			timestamp;
	unsigned int			flag;
	int				clientcount;
	unsigned int			autofixhistory[MAXAUTOFIX];
	unsigned int			manualfixhistory[MAXMANUALFIX];
	int				chanopcount;
	int				chanopmax;
	int				channotecount;
	struct OCF_chanop*		chanop;		
	struct OCF_channote*		channote[MAXNOTECOUNT];
};


struct OCF_topops
{
	int			topscores[OPCOUNT];
	int			currentopscores[OPCOUNT];
	int			currentnopscores[OPCOUNT];

	unsigned int		autofixhistory[MAXAUTOFIX];
	unsigned int		manualfixhistory[MAXMANUALFIX];
};

/* Sets the current day. This function must be called right after
 * DB_load() and before the first DB_write(). */
void DB_set_day(int daystamp);


/* channel name currently read from (optimizes multiple read queries to same channel) */
int   DB_set_read_channel( char* channel );
char* DB_get_read_channel();

/* channel name currently written to (updated, op registration) */
int   DB_set_write_channel( char* channel );
char* DB_get_write_channel();

/* returns 1 if channel exists in our database, otherwise 0 */
int DB_channel_exists( char* channel );

/* Test if channel exists in our database, and return number of registered chanops in the database */
int DB_channel_get_number_of_userhosts( char* channel );


/* char strings with user and host */
void DB_register_op( char* user, char* host );

/* registers manual fix with current write channel */
void DB_register_manualfix( unsigned int timestamp );

/* registers auto fix with current write channel */
void DB_register_autofix( unsigned int timestamp );


/* Must be called once per 24 hours, to update channel etc. internal counters (e.g. just after midnight) */
void DB_update_day( int daystamp );


/* Should be called as often as we want the channels backed up on disk */
void DB_save();

/* Should ALWAYS be called at startup */
void DB_load();


/* channel name, and a caller-reserved OCF_topops structure which will be filled by the function */
int DB_poll_channel( struct OCF_topops* result, char** opusers, char** ophosts, int num_opuserhosts, 
		     char** nopusers, char** nophosts, int num_nopuserhosts );

/* Simple score checker for some of the opping logic - fills a caller-created structure with N top op scores */
int DB_get_op_scores( int* score, int scorecount );

/* Get N top scorers matching a passed users/hosts list */
int DB_get_top_user_hosts( struct OCF_chanuser* op_array, char** users, char** hosts, int num_userhosts, int max );

/* Get N top scorers in the database, for seeding OPLIST */
int DB_get_oplist( struct OCF_chanuser* chanusers, int num_users );


/* Note and flag functions (all work with current read channel or write channel) */

/* Adds note with user id, timestamp and string text */
int DB_channel_add_note( unsigned int id, unsigned int timestamp, char* note );

/* Clean note #, with ids ranging from 1 (first) to MAXNOTECOUNT (last) */
int DB_channel_del_note( int count );

/* Returns the amount of notes for current read channel, from 1 to MAXNOTECOUNT */
int DB_channel_note_count();

/* Returns a pointer to an internal OCF_channote record which contains note information for read channel note # */
struct OCF_channote* DB_channel_get_note( int count );

/* Sets flag for current write channel */
void DB_channel_set_flag( unsigned int flag );

/* Returns flag for current read channel */
unsigned int DB_channel_get_flag();

