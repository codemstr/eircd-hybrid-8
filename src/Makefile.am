AUTOMAKE_OPTIONS = foreign

bin_PROGRAMS = ircd

AM_YFLAGS = -d

AM_CPPFLAGS = $(LTDLINCL) -I$(top_srcdir)/include
ircd_LDFLAGS = -export-dynamic
ircd_LDADD = $(LIBLTDL)
ircd_DEPENDENCIES = $(LTDLDEPS)

ircd_SOURCES = auth.c            \
               channel.c         \
               channel_mode.c    \
               client.c          \
               conf.c            \
               conf_class.c      \
               conf_db.c         \
               conf_pseudo.c     \
               conf_parser.y     \
               conf_lexer.l      \
               dbuf.c            \
               event.c           \
               fdlist.c          \
               getopt.c          \
               gline.c           \
               hash.c            \
               hostmask.c        \
               ipcache.c         \
               irc_string.c      \
               ircd.c            \
               ircd_signal.c     \
               list.c            \
               listener.c        \
               log.c             \
               match.c           \
               memory.c          \
               mempool.c         \
               misc.c            \
               modules.c         \
               motd.c            \
               numeric.c         \
               packet.c          \
               parse.c           \
               s_bsd_epoll.c     \
               s_bsd_poll.c      \
               s_bsd_devpoll.c   \
               s_bsd_kqueue.c    \
               res.c             \
               reslib.c          \
               restart.c         \
               resv.c            \
               rng_mt.c          \
               rsa.c             \
               s_bsd.c           \
               send.c            \
               server.c          \
               user.c            \
               version.c         \
               watch.c           \
               whowas.c
