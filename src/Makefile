CC      = gcc
PROF    = 
NOCRYPT = 
C_FLAGS = -O -Wall -g -ggdb3 -Dlinux $(PROF) $(NOCRYPT)
L_FLAGS = -O $(PROF)


SRC = *.c

INC = *.h



O_FILES = act_comm.o act_info.o act_move.o act_obj.o act_wiz.o comm.o const.o \
          db.o fight.o handler.o interp.o magic.o save.o special.o update.o project.o \
          kav_info.o kav_fight.o kav_wiz.o clan.o  mage.o rot_wiz.o \
          demon.o wizutil.o low_act.o dempowers.o drow.o vamp.o warlock.o\
	  leader.o arena.o config.o ninja.o monk.o soulstrike.o temp.o \
	  garou.o olc.o string.o bit.o board.o bounty.o ar_plr.o wraith.o\
	  kingdom.o clan_list.o low_wiz.o low_misc.o ar_wiz.o highlander.o\
          hunter.o mal_war.o fae2.o changes.o dragon.o traits.o $(IMC_OFILES)
        low: $(O_FILES)
	rm -f merc
	$(CC) $(L_FLAGS) -o merc $(O_FILES) -lm -lcrypt

.c.o: merc.h
	$(CC) -c $(C_FLAGS) $<

checkin:
	ci -l $(SRC) $(INC)

diff:	
	rcsdiff -u $(SRC) $(INC) > diff.new
	gzip diff.new

store:
	cp $(SRC) $(INC) ./backup
