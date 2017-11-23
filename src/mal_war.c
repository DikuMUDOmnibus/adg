/*

  War Snippet
  Copyright ©2001 Malawar
  The Realm of Lost Souls
  telnet://tamarisk.ab.ca:9595
  soon to be telnet://mud.malawar.net:9595

  Description:  It's War.. hooh.. yeah...
                Basically, War is a battle between two or more players.
                The loser suffers no consequences, other than a damaged
                ego. The winner, however, receives a reward.

                There are two types of war in this code(I can't make it
                too advanced.. my MUD needs some unique-ness to it :):

                1. Last Man Standing:   Basically, this war goes on until
                                        there is only one person remaining.
                                        Players receive a reward for each
                                        person eliminated, and the winner
                                        gets a bonus.

                2. Team Deathmatch:     Two teams duke it out in the style
                                        of Last Man Standing. Players are
                                        divided between two teams, and the
                                        team that survives the war wins.

  Comment:      This is the first snippet that I've written specifically
                for release.  I lack the time to test it thoroughly, and
                it may contain bugs. I made this code so it would fit in
                almost any type of MUD derived from diku. I tried to make
                it as simple as possible for the end-user to install.

  Disclaimer:   I am not responsible for any adverse affects of this code.
                This includes, but is not restricted to: Loss of life,
                Unemployment, Diarrhea, Nausea, Lack of Players, Theft of
                Players, Loss of Hair, etcetera, etcetera. Don't blame me.

  Agreement:    You may use this code if and only if you give visible credit
                to me (Malawar), within the login screen (that'd be nice), or
                for you miserly people who don't like boosting my ego too much..
                add it in the helpfile. It should contain something like..

                War Code ©2001 Malawar
                http://www.malawar.net/mudding

                If you do not do this, you may NOT use this code. Nuff said.

  Instructions: As with any major (or minor) code change, it is always wise
                to back your code up first.

                1.  Add the following code block into merc.h, preferably towards the bottom

                    // Begin Snip

                    void UpdateWar(void);
                    bool InWar(CHAR_DATA *ch);

                    #define CHECK_WAR(_character) { if(InWar((_character))) { \
                              send_to_char("Not while in a war.\n\r",(_character)); return; }

                    void do_startwar(CHAR_DATA *ch, char *argument);
                    void do_endwar(CHAR_DATA *ch, char *argument);
                    void do_warstats(CHAR_DATA *ch, char *argument);
                    void do_joinwar(CHAR_DATA *ch, char *argument);
                    void do_eliminate(CHAR_DATA *ch, char *argument);
                    // End Snip

                2.  Add do_startwar, do_endwar, do_warstats, do_joinwar, and do_eliminate to your
                    command table (Usually found in interp.c). Startwar and Endwar should be
                    immortal commands, while warstats, joinwar and eliminate should be available to mortals.
                    For example, I would implement it like this:

                    // Begin Snip
                    { "startwar",     do_startwar, POS_DEAD,    LEVEL_IMMORTAL, LOG_ALWAYS},
                    { "endwar",       do_endwar,   POS_DEAD,    LEVEL_IMMORTAL, LOG_ALWAYS},

                    { "joinwar",      do_joinwar,  POS_SLEEPING, LEVEL_MORTAL,  LOG_NORMAL},
                    { "warstats",     do_warstats, POS_SLEEPING, LEVEL_MORTAL,  LOG_NORMAL},
                    { "eliminate",    do_eliminate,POS_SLEEPING, LEVEL_MORTAL,  LOG_NORMAL},
                    // End Snip

                3.  In update.c, within the update_handler function, after all the variable
                    declarations (before any other update code), add the following line:

                    UpdateWar();

                4.  Go back into merc.h, and add the following line to the bottom of
                    struct  pc_data:

                    int war_bits;  // War Bits. Duh.

                5.  Go into all the functions and commands that you don't want players executing
                    while in a war (Such as do_recall, do_quit, do_decap, etc) and add the following
                    line somewhere near the top, after variables are declared:

                    CHECK_WAR(ch);

                6.  Edit the #defines below to customize your war experience. Most are self-explanitory.

                7.  Upload this file, and add it to your Makefile. I.E. add mal_war.o to the O_FILES..

                7.  Do a clean compile. (I.E. rm *.o then make)

                8.  Upload mal_war.hlp to your area directory, and add it to your area.lst file.
                    (You may have to modify level restrictions on the helpfiles..)

                9.  Reboot. Enjoy.
*/

#define MWAR_LVNUM           50            // Lower VNUM of the area the war is held in.
#define MWAR_UVNUM           67            // Upper VNUM of the area the war is held in.

#define ENDVNUM         3054            // The VNUM you wish the players to be at when the
// war is over

#define TEAMONE         "#rAlpha#e"     // Name of team one, for team games.
#define TEAMTWO         "#bBravo#e"     // Name of team two, for team games.

#define WARSTRING       "{D[{RWAR{D]{w %s#n\n\r" // The string for war announcements. %s is replaced with the
// Message. You can do whatever you want here.

#define REWARDVAR       ->pcdata->quest   // The variable for rewards. Here it is set up for quest points
// This translates into ch->pcdata->quest, for example.
#define REWARDNAM       "{CQuest Points{w" // What do you call your reward?

#define MWAR_MINLEVEL        LEVEL_AVATAR    // Minimum level to enter a war. Can be numerical.

/*
    You do not have to change anything below this line. This should run "out of the box".
    Truly catered to the GodWars community. :)
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "merc.h"

#define MWAR_REWARD(_character,_amt) { (_character)REWARDVAR+=_amt; }



bool InWar(CHAR_DATA *ch);
void EndWar(void);
void StartWar(void);
void WarAnnounce(char *argument);


int war_init=0;     // Did we zero out all the war stuff yet?
int joining=0;
// The Stages of War.
#define WAR_NONE     0      // No war. This bit for aesthetic purposes only.
#define WAR_STARTING 1      // We are in the pre-war countdown
#define WAR_FIGHTING 2      // And the Allied forces stormed the beach..

// Bits of War.. for Players
#define PWAR_JOIN  1
#define PWAR_BATT  1<<1
#define PWAR_ELIM  1<<2
#define PWAR_TEAM1 1<<3
#define PWAR_TEAM2 1<<4

// Misc. #defines to fit my coding style.

#ifndef MSL
    #define MSL MAX_STRING_LENGTH
#endif

#ifndef MIL
    #define MIL MAX_INPUT_LENGTH
#endif

#ifndef stc
    #define stc send_to_char
#endif

/* Clears the War */
void ClearWar(void)
{
    mal_war.timer = 0;
    mal_war.type = 0;
    mal_war.bonus = 0;
    mal_war.reward = 0;
    mal_war.stage = 0;
    war_init = 1;
    return;
}

/* For messages concerning the war. */
void WarAnnounce(char *argument)
{
    DESCRIPTOR_DATA *d;
    char buf[MSL];

    if(!argument)return;

    sprintf(buf,WARSTRING,argument);

    for(d=descriptor_list;d;d=d->next)
    {
        if(!d->character || d->connected!=CON_PLAYING)
            continue;

        stc(buf,d->character);
    }

}


void do_startwar(CHAR_DATA *ch, char *argument)
{
    char arg[MIL];
    char buf[MSL];
    int type,reward,time;

    joining=0;

    if(IS_NPC(ch))
        return;

    if(!war_init) ClearWar();

    if(mal_war.stage != 0)
    {
        stc("There is already a war going.\n\r",ch);
        return;
    }

    argument=one_argument(argument,arg);

    if(arg[0]=='\0' || !is_number(arg))
    {
        stc("Syntax:    StartWar <Type> <Reward Amount> <Time till Start>\n\r"
            "\n\r"
            "           Valid types are:\n\r"
            "           [1] Last Man Standing\n\r"
            "           [2] Team Deathmatch\n\r",ch);
        return;
    }
    type=atoi(arg);
    if(type<1 || type>2)
    {
        do_startwar(ch,"");
        return;
    }

    argument=one_argument(argument,arg);

    if(arg[0]=='\0' || !is_number(arg))
    {
        do_startwar(ch,"");
        return;
    }
    reward=atoi(arg);

    argument=one_argument(argument,arg);

    if(arg[0]=='\0' || !is_number(arg))
    {
        do_startwar(ch,"");
        return;
    }
    time=atoi(arg);

    sprintf(buf,"{wA {RWar{w will be starting in {R%d{w seconds. Type '#wWarstats{w' for more info, or '#wJoinwar{w' to join.{x",time);
    WarAnnounce(buf);

    mal_war.timer=time;
    mal_war.type=type;
    mal_war.reward=reward;
    mal_war.stage=WAR_STARTING;
}

void UpdateWar(void)
{
    static int pulse_war;

    if(--pulse_war<=0)
    {
        pulse_war=PULSE_PER_SECOND;
    }
    else return;

    if(!war_init)ClearWar();

    if(mal_war.stage==WAR_NONE)return; /* No war, no update. Boy, am I smart. */

    if(mal_war.stage==WAR_STARTING)
    {
        mal_war.timer--;
        if(mal_war.timer<=0)
        {
            StartWar();
            return;
        }

        if((mal_war.timer>5 && mal_war.timer%5 == 0) || mal_war.timer <=5 )
        {
            char buf[MSL];
            sprintf(buf,"{wThe {RWar{w will start in {R%d{w seconds.{x",mal_war.timer);
            WarAnnounce(buf);
            return;
        }
    }

    else if(mal_war.stage==WAR_FIGHTING)
    {
        mal_war.timer++; /* Let's keep track of time for the hell of it. */
        return;
    }
    else
    {
        bug("This War is BUGGED!",0);
        return;
    }

}

void StartWar(void)
{
    DESCRIPTOR_DATA *d;
    ROOM_INDEX_DATA *r;
    int team=0;
    char team1[MSL];
    char team2[MSL];
    char buf[MSL];

    team1[0]='\0';
    team2[0]='\0';

    if(mal_war.stage==0)
        return;

    if(joining<2)
    {
        WarAnnounce("#rNot enough players for the war.#n");
        EndWar();
//        ClearWar();
        return;
    }

    WarAnnounce("{wThe {RWar{w has begun!{x");

    for(d=descriptor_list;d;d=d->next)
    {
        if(!d->character||!d->character->pcdata)continue;
        if(!IS_SET(d->character->pcdata->war_bits,PWAR_JOIN))continue;

        d->character->pcdata->war_bits=PWAR_BATT;

        if(mal_war.type==2)
        {
            if(team)
            {
                SET_BIT(d->character->pcdata->war_bits,PWAR_TEAM1);
                strcat(team1,d->character->name);
                strcat(team1," ");
            }
            else
            {
                SET_BIT(d->character->pcdata->war_bits,PWAR_TEAM2);
                strcat(team2,d->character->name);
                strcat(team2," ");
            }

            team=!team;
        }

        do
        {
            r=get_room_index(number_range(MWAR_LVNUM,MWAR_UVNUM));
        }
        while (!r);

        act("$n is whisked into the war.",d->character,NULL,NULL,TO_ROOM);
        stc("You are whisked into the war.\n\r",d->character);

        char_from_room(d->character);
        char_to_room(d->character,r);

        act("$n lands at your feet.",d->character,NULL,NULL,TO_ROOM);
        do_look(d->character,"");

    }
    if(mal_war.type==2)
    {
        sprintf(buf,"{wThe "TEAMONE" {wteam{D: {W%s#n",team1);
        WarAnnounce(buf);
        sprintf(buf,"{wThe "TEAMTWO" {wteam{D: {W%s#n",team2);
        WarAnnounce(buf);
    }

    mal_war.stage=WAR_FIGHTING;
}

void EndWar(void)
{
    ROOM_INDEX_DATA *r;
    DESCRIPTOR_DATA *d;

    if(mal_war.stage==0)
        return;

    WarAnnounce("#eThe War has Ended.#n");

    r=get_room_index(ENDVNUM);
    if(!r)
    {
        bug("The War ending room does not exist!",0);
        return;
    }

    for(d=descriptor_list;d;d=d->next)
    {
        if(!d->character||!d->character->pcdata)continue;
        if(d->character->pcdata->war_bits==0)continue;
        if(!IS_SET(d->character->pcdata->war_bits,PWAR_BATT))
        {
            d->character->pcdata->war_bits=0;
            continue;
        }
        d->character->pcdata->war_bits=0;
        char_from_room(d->character);
        char_to_room(d->character,r);
 	d->character->fight_timer = 0;
        do_restore(d->character,"self");
        act("$n arrives in a flash of white light.",d->character,NULL,NULL,TO_ROOM);
        stc("You are removed from the Wargrounds.\n\r",d->character);
    }
    ClearWar();
}

void do_endwar(CHAR_DATA *ch, char *argument)
{
    if(IS_NPC(ch))return;

    if(mal_war.stage==0)
    {
        stc("There is no war going on.\n\r",ch);
        return;
    }

    EndWar();
}

void do_joinwar(CHAR_DATA *ch, char *argument)
{
    char buf[MSL];

    if(IS_NPC(ch)||!ch->pcdata) return;

    if(ch->level<MWAR_MINLEVEL)
    {
        stc("You are not a high enough level to join the war.\n\r",ch);
        return;
    }
/* added to stop you from joining when a war isn't running */
    if(mal_war.stage==0)
    {
        stc("There is no war.\n\r",ch);
        return;
    }

/* added to keep imms out of war */
    if( IS_IMMORTAL(ch) )
    {
        stc("Immortals are not permitted into wars.\n\r", ch );
        return;
    }


    if(ch->pcdata->war_bits)
    {
        stc("You have already joined that war.\n\r",ch);
        return;
    }

    if(mal_war.stage==2)
    {
        stc("It is too late to join that war.\n\r",ch);
        return;
    }

    ch->pcdata->war_bits=PWAR_JOIN;

    mal_war.bonus+=mal_war.reward/2;

    sprintf(buf,"{W%s{w has joined the war. Reward incrased to {R%d{w "REWARDNAM".{x",ch->name,mal_war.bonus);
    joining++;
    WarAnnounce(buf);
}

void do_warstats(CHAR_DATA *ch, char *argument)
{
    char buf[MSL];
    char const *wstatus;
    DESCRIPTOR_DATA *d;
    int hp_stats = 0;
    int mana_stats = 0;
    int move_stats = 0;

    if(IS_NPC(ch))return;
    if(mal_war.stage==0)
    {
        stc("There is no war.\n\r",ch);
        return;
    }

    sprintf(buf,
            "#e=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=#n\n\r",
            "{cWar Status{D:{w\n\r"
            "\n\r"
            "{cType{D:      {w%s#n\n\r"
            "{cReward{D:    {w%d {cWar Bonus{D:{w %d#n\n\r"
            "{c%s {w%d#n\n\r"
            "#e=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=#n\n\r",
            mal_war.type==1?"Last Man Standing":"Team Deathmatch",
            mal_war.reward,
            mal_war.bonus,
            mal_war.stage==WAR_STARTING?"Starts In:":"Running:  ",
            mal_war.timer);
    stc(buf,ch);

    send_to_char("{CPeople in this war:#n\n\r",ch);
    send_to_char("{cName            {C    Health{c   Stamina {C    Mana {c   War Status#n\n\r",ch);
    for ( d = descriptor_list; d != NULL; d = d->next )
    {
        if (d->character != NULL)
        {
            if (d->character->in_room != NULL)
            {
                if (d->character->pcdata->war_bits == NULL) continue;
                if (d->character->max_hit > 0)  hp_stats   = 100 * d->character->hit / d->character->max_hit;
                if (d->character->max_move > 0) move_stats = 100 * d->character->move / d->character->max_move;
                if (d->character->max_mana > 0) mana_stats = 100 * d->character->mana / d->character->max_mana;
                switch( d->character->pcdata->war_bits )
                {
                              default:         wstatus = ""; break;
                              case PWAR_JOIN:  wstatus = "#gJoining#n   "; break;
                              case PWAR_BATT:  wstatus = "#gFighting#n  "; break;
                              case PWAR_ELIM:  wstatus = "#gEliminated#n"; break;
                              case PWAR_TEAM1: wstatus = "#rAlpha     #n"; break;
                              case PWAR_TEAM2: wstatus = "#bBravo     #n"; break;
                }
                sprintf(buf,"{c%-15s    {C%3d#e/{C100   {c%3d#e/{c100 {C  %3d#e/#c100   {D[{c%s{D]#n\n\r",
                    d->character->name, hp_stats, move_stats, mana_stats, wstatus);
                send_to_char(buf,ch);
            }
        }
    }
    send_to_char("#e=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=#n\n\r", ch );
    return;
}


void do_eliminate(CHAR_DATA *ch,char *argument)
{
    char arg[MIL];
    char buf[MSL];
    CHAR_DATA *vch;

    if(IS_NPC(ch))return;
    if(!ch->pcdata)return;

    if(!IS_SET(ch->pcdata->war_bits,PWAR_BATT))
    {
        stc("You are not in the war.\n\r",ch);
        return;
    }

    argument=one_argument(argument,arg);

    if(arg[0]=='\0')
    {
        stc("Who do you wish to eliminate?\n\r",ch);
        return;
    }

    if((vch=get_char_room(ch,arg))==NULL)
    {
        stc("They are not here.\n\r",ch);
        return;
    }

    if(IS_NPC(vch) || !vch->pcdata)
    {
        stc("Not on them. Dimwit.\n\r",ch);
        return;
    }

    if(!IS_SET(vch->pcdata->war_bits,PWAR_BATT))
    {
        stc("They are not in the war.\n\r",ch);
        return;
    }

    if(IS_SET(ch->pcdata->war_bits,PWAR_TEAM1) &&IS_SET(vch->pcdata->war_bits,PWAR_TEAM1))
    {
        stc("They are on your team.\n\r",ch);
        return;
    }
    if(IS_SET(ch->pcdata->war_bits,PWAR_TEAM2) &&IS_SET(vch->pcdata->war_bits,PWAR_TEAM2))
    {
        stc("They are on your team.\n\r",ch);
        return;
    }

    if(vch->hit>0)
    {
        stc("They must be mortally wounded.\n\r",ch);
        return;
    }

    sprintf(buf,"{W%s {whas eliminated {W%s{w from the war, and has received {R%d{w "REWARDNAM".#n",
            ch->name,vch->name,mal_war.reward);

    WarAnnounce(buf);

    MWAR_REWARD(ch,mal_war.reward);

    REMOVE_BIT(vch->pcdata->war_bits,PWAR_BATT);
    SET_BIT(vch->pcdata->war_bits,PWAR_ELIM);

    {
        ROOM_INDEX_DATA *r;
        r=get_room_index(ENDVNUM);
        act("$n is eliminated from the war.",vch,NULL,NULL,TO_ROOM);
	ch->fight_timer = 0;
        char_from_room(vch);
        char_to_room(vch,r);
        act("$n appears in the room.",vch,NULL,NULL,TO_ROOM);
        do_restore(vch,"self");
        do_look(vch,"auto");
    }

    {
        DESCRIPTOR_DATA *d;
        int playaz=0;

        for(d=descriptor_list;d;d=d->next)
        {
            if(!d->character || !d->character->pcdata)
                continue;
            if(!IS_SET(d->character->pcdata->war_bits,PWAR_BATT))continue;

            if(mal_war.type==1)
            {
                if(d->character!=ch)playaz++;
            }
            else if(mal_war.type==2)
            {
                if(IS_SET(d->character->pcdata->war_bits,PWAR_TEAM1) && IS_SET(ch->pcdata->war_bits,PWAR_TEAM2))playaz++;
                if(IS_SET(d->character->pcdata->war_bits,PWAR_TEAM2) && IS_SET(ch->pcdata->war_bits,PWAR_TEAM1))playaz++;
            }
        }

        if(!playaz)
        {
            int teamnum=0; // For my laziness
            if(mal_war.type==1)
            {
                sprintf(buf,"{W%s{w has won the war. The bonus of {R%d{w "REWARDNAM" {wis theirs.#n",ch->name,mal_war.bonus);
                MWAR_REWARD(ch,mal_war.bonus);

                WarAnnounce(buf);
                EndWar();
            }
            else if(mal_war.type==2)
            {
                sprintf(buf,"{wThe{W %s {wteam has won the war. The bonus of {R%d{w "REWARDNAM" is theirs.#n",
                        (teamnum=IS_SET(ch->pcdata->war_bits,PWAR_TEAM2))?TEAMTWO:TEAMONE,mal_war.bonus);
                WarAnnounce(buf);

                for(d=descriptor_list;d;d=d->next)
                {
                    if(!d->character||!d->character->pcdata)
                        continue;

                    if(teamnum)
                    {
                        if(IS_SET(d->character->pcdata->war_bits,PWAR_TEAM2))
                            {MWAR_REWARD(d->character,mal_war.bonus);}
                        else continue;
                    }
                    else
                    {
                        if(IS_SET(d->character->pcdata->war_bits,PWAR_TEAM1))
                            {MWAR_REWARD(d->character,mal_war.bonus);}
                        else continue;
                    }
                }




                EndWar();
            }
        }
    }


}

bool InWar(CHAR_DATA *ch)
{
    if(IS_NPC(ch)||!ch->pcdata)return FALSE;

    if(!IS_SET(ch->pcdata->war_bits,PWAR_BATT))return TRUE;

    return FALSE;
}
