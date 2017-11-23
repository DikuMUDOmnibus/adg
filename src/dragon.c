/********************************************************
 *                                                      *
 * Coded by Corey Olney(Kip) and Justin C. Good(bohica) *
 * If you are going to use this code and did not get it *
 * from me, please tell me on ICQ #21355940 or AOL IM   *
 * kippykp.                                             *
 * (copyrighted 1998-1999)                              *
 * also send me improvements you make.                  *
 *                                                      *
 ********************************************************/

#if defined(macintosh)
#include <types.h>
#else
#include <sys/types.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "merc.h"
#include "player.h"
#include "warlock.h"
#include "dragon.h"

void do_drackform( CHAR_DATA *ch, char *argument)
{
    char arg [MAX_INPUT_LENGTH];
    char buf [MAX_STRING_LENGTH];
    int type = 0;

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;
    if( !IS_CLASS(ch, CLASS_DRAGON))
    {
	stc( "Huh?\n\r",ch);
	return;
    }
    if( ch->pcdata->dragonage < YOUNG)
    {
	stc( "You need to be at least a young dragon to use this.\n\r",ch);
	return;
    }

    if( IS_SET(ch->extra, EXTRA_DRAGON) )
    {
	stc( "You only can get outa vampire dragon form with dragon.\n\r",ch);
	return;
    }
    
    if (IS_POLYAFF(ch, POLY_DRAGON))
    {
        REMOVE_BIT(ch->polyaff, POLY_DRAGON);
	REMOVE_BIT(ch->affected_by, AFF_POLYMORPH);
        act( "You transform into human form.", ch, NULL, NULL, TO_CHAR );
        act( "$n's shrinks back into human form.", ch, NULL, NULL, TO_ROOM );
        ch->max_hit = ch->max_hit - 2500;
        ch->hit = ch->hit - 2500;
        if (ch->hit < 1) ch->hit = 1;
        ch->damroll -= 500;
        ch->hitroll -= 500;
 	free_string( ch->morph );
        ch->morph = str_dup( "" );
        return;
    }
    else if (IS_AFFECTED(ch,AFF_POLYMORPH))
    {
	send_to_char("You cant demon form when changed.\n\r",ch);
	return;
    }
    if (ch->stance[0] != -1) do_stance(ch,"");
    if (ch->mounted == IS_RIDING) do_dismount(ch,"");

    type = atoi(arg);
    if( type > 5 || type < 1 )
    {
	stc( "please choose a number between 1 and 5.\n\r",ch);
	return;
    }
    switch(type)
    {
	case 1:
		sprintf(buf, "%s the huge green dragon", ch->name);
		free_string( ch->morph );
		ch->morph = str_dup( buf );
		break;
	case 2:
		sprintf(buf, "%s the huge black dragon", ch->name);
		free_string( ch->morph );
		ch->morph = str_dup( buf );
		break;
	case 3:
		sprintf(buf, "%s the huge red dragon", ch->name);
		free_string( ch->morph );
		ch->morph = str_dup( buf );
		break;
	case 4:
		sprintf(buf, "%s the huge blue dragon", ch->name);
		free_string( ch->morph );
		ch->morph = str_dup( buf );
		break;
	case 5:
		sprintf(buf, "%s the huge gold dragon", ch->name);
		free_string( ch->morph );
		ch->morph = str_dup( buf );
		break;
    }
    act( "You transform into a huge dragon.", ch, NULL, NULL, TO_CHAR );
    act( "$n's body grows and distorts into a huge dragon.", ch, NULL, NULL, TO_ROOM );
    ch->pcdata->mod_str = 15;
    ch->pcdata->mod_dex = 15;
    SET_BIT(ch->polyaff, POLY_DRAGON);
    SET_BIT(ch->affected_by, AFF_POLYMORPH);

    ch->max_hit += 2500;
    ch->hit     += 2500;
    ch->damroll += 500;
    ch->hitroll += 500;
    return;
}

const struct dage_type dage_table[] = 
{  
    { "Young",        YOUNG,        1000000   },
    { "Juvenile",     JUVENILE,     2000000   },
    { "Venerable",    VENERABLE,    4000000   },
    { "Very Young",   MATURE_ADULT, 8000000   },
    { "Great Wyrm",   GREAT_WYRM,   16000000  },
    { "Young Adult",  VERY_YOUNG,   32000000  },
    { "Mature Adult", OLD,          64000000  },
    { "Adult",        YOUNG_ADULT,  128000000 },
    { "Old",          ADULT,        256000000 },
    { NULL,           -1,           0         }
};

void do_age( CHAR_DATA *ch, char *argument )
{
     char arg[MAX_INPUT_LENGTH];
     char buf[MAX_STRING_LENGTH];
     int i;

    argument = one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
    do_age(ch,"show");
    return;
    }

    if (IS_NPC(ch)) return;
    if (!IS_CLASS(ch, CLASS_DRAGON)) { stc("Huh?\n\r",ch); return;}

    if (!str_cmp(arg,"show"))
    {
stc("{D-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n\r",ch);
stc("{w     To gain an age type:\n\r",ch);
stc("{R           age gain\n\r",ch);
stc("{D-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n\r",ch);
         for (i=0; dage_table[i].bit != -1; i++)
         {
            if (ch->pcdata->dragonage >= dage_table[i].bit) continue;
            sprintf(buf," {B%12s {D-{c %9d {Bexp{x\n\r",dage_table[i].name,dage_table[i].cost);
            stc(buf,ch);
         }      
stc("{D-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n\r",ch);
        return;
    }

    if (!str_cmp(arg,"reset"))
    { ch->pcdata->dragonage = 0; }

    if (!str_cmp(arg,"gain"))
    {
       if ( ch->pcdata->dragonage == 0)
       {
           if (ch->exp < dage_table[0].cost) { stc("You don't have the required exp points.\n\r",ch); return; }
           ch->exp -= dage_table[0].cost;
           ch->pcdata->dragonage = dage_table[0].bit;
           stc("You gain a dragon age!\n\r",ch);
sprintf(buf,"%s is now a %s dragon!\n\r",ch->name,dage_table[ch->pcdata->dragonage-1].name);
do_info(ch,buf);
return;
       }
       else if ( ch->pcdata->dragonage == dage_table[0].bit)
       {
           if (ch->exp < dage_table[1].cost) { stc("You don't have the required exp points.\n\r",ch); return; }
           ch->exp -= dage_table[1].cost;
           ch->pcdata->dragonage = dage_table[1].bit;
           stc("You gain a dragon age!\n\r",ch);
sprintf(buf,"%s is now a %s dragon!\n\r",ch->name,dage_table[ch->pcdata->dragonage-1].name);
do_info(ch,buf);
           return;
       }
       else if ( ch->pcdata->dragonage == dage_table[1].bit)
       {
           if (ch->exp < dage_table[2].cost) { stc("You don't have the required exp points.\n\r",ch); return; }
           ch->exp -= dage_table[2].cost;
           ch->pcdata->dragonage = dage_table[2].bit;
           stc("You gain a dragon age!\n\r",ch);
sprintf(buf,"%s is now a %s dragon!\n\r",ch->name,dage_table[ch->pcdata->dragonage-1].name);
do_info(ch,buf);
           return;
       }
       else if ( ch->pcdata->dragonage == dage_table[2].bit)
       {
           if (ch->exp < dage_table[3].cost) { stc("You don't have the required exp points.\n\r",ch); return; }
           ch->exp -= dage_table[3].cost;
           ch->pcdata->dragonage = dage_table[3].bit;
           stc("You gain a dragon age!\n\r",ch);
sprintf(buf,"%s is now a %s dragon!\n\r",ch->name,dage_table[ch->pcdata->dragonage-1].name);
do_info(ch,buf);
           return;
       }
       else if ( ch->pcdata->dragonage == dage_table[3].bit)
       {
           if (ch->exp < dage_table[4].cost) { stc("You don't have the required exp points.\n\r",ch); return; }
           ch->exp -= dage_table[4].cost;
           ch->pcdata->dragonage = dage_table[4].bit;
           stc("You gain a dragon age!\n\r",ch);
sprintf(buf,"%s is now a %s dragon!\n\r",ch->name,dage_table[ch->pcdata->dragonage-1].name);
do_info(ch,buf);
           return;
       }
       else if ( ch->pcdata->dragonage == dage_table[4].bit)
       {
           if (ch->exp < dage_table[5].cost) { stc("You don't have the required exp points.\n\r",ch); return; }
           ch->exp -= dage_table[5].cost;
           ch->pcdata->dragonage = dage_table[5].bit;
           stc("You gain a dragon age!\n\r",ch);
sprintf(buf,"%s is now a %s dragon!\n\r",ch->name,dage_table[ch->pcdata->dragonage-1].name);
do_info(ch,buf);
           return;
       }
       else if ( ch->pcdata->dragonage == dage_table[5].bit)
       {
           if (ch->exp < dage_table[6].cost) { stc("You don't have the required exp points.\n\r",ch); return; }
           ch->exp -= dage_table[6].cost;
           ch->pcdata->dragonage = dage_table[6].bit;
           stc("You gain a dragon age!\n\r",ch);
sprintf(buf,"%s is now a %s dragon!\n\r",ch->name,dage_table[ch->pcdata->dragonage-1].name);
do_info(ch,buf);
           return;
       }
       else if ( ch->pcdata->dragonage == dage_table[6].bit)
       {
           if (ch->exp < dage_table[7].cost) { stc("You don't have the required exp points.\n\r",ch); return; }
           ch->exp -= dage_table[7].cost;
           ch->pcdata->dragonage = dage_table[7].bit;
           stc("You gain a dragon age!\n\r",ch);
sprintf(buf,"%s is now a %s dragon!\n\r",ch->name,dage_table[ch->pcdata->dragonage-1].name);
           return;
       }
       else if ( ch->pcdata->dragonage == dage_table[7].bit)
       {
           if (ch->exp < dage_table[8].cost) { stc("You don't have the required exp points.\n\r",ch); return; }
           ch->exp -= dage_table[8].cost;
           ch->pcdata->dragonage = dage_table[8].bit;
           stc("You gain a dragon age!\n\r",ch);
sprintf(buf,"%s is now a %s dragon!\n\r",ch->name,dage_table[ch->pcdata->dragonage-1].name);
do_info(ch,buf);
           return;
       }
       else if ( ch->pcdata->dragonage == dage_table[8].bit)
       {
           stc("You are already as old as you can get!.\n\r",ch); return;
       }
    }
//   return;
}

void do_hatch( CHAR_DATA *ch, char *argument )
{
	CHAR_DATA *victim;
	char arg [MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );

	if (IS_NPC(ch)) return;

	if( !IS_IMMORTAL(ch) )
	{
		if( !IS_CLASS(ch, CLASS_DRAGON ) )
		{
			send_to_char( "Huh?\n\r", ch);
			return;
		}
		if( ch->pcdata->dragonage < GREAT_WYRM )
		{
			send_to_char( "You must be a great wyrm to create hatchlings.\n\r", ch );
			return;
		}
		if( ch->pcdata->hatch > 2 && ch->pcdata->stats[UNI_GEN] > 2 )
		{
			send_to_char( "You have already created 2 hatchlings.\n\r", ch );
			return;
		}
    if( ch->class != CLASS_DRAGON )       
    {
        stc( "Huh?\n\r",ch);
        return;
    }
	}

    if ( arg[0] == '\0' )
    {
    send_to_char( "Hatch whom?\n\r", ch );
    return;
    }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
    send_to_char( "They aren't here.\n\r", ch );
    return;
    }

    if ( IS_NPC(victim) )
    {
    send_to_char( "Not on NPC's.\n\r", ch );
    return;
    }

    if ( ch == victim )
    {
    send_to_char( "You cannot hatch yourself.\n\r", ch );
    return;
    }

    if (!IS_IMMUNE(victim,IMM_DEMON))
    {
	send_to_char( "Not on an unwilling person.\n\r", ch);
	return;
    }

    if ( victim->level != LEVEL_AVATAR && !IS_IMMORTAL(victim) )
    {
    send_to_char( "You can only hatch a dragon from avatars.\n\r", ch );
    return;
    }

	if( IS_CLASS( victim, CLASS_DEMON ) )
	{
		send_to_char( "You can't hatch a dragon from a Demon.\n\r", ch );
		return;
	}
	if( IS_CLASS( victim, CLASS_MAGE ) )
	{
		send_to_char( "You can't hatch a dragon from a Mage.\n\r", ch );
		return;
	}
	if( IS_CLASS( victim, CLASS_WEREWOLF ) )
	{
		send_to_char( "You can't hatch a dragon from a Werewolf.\n\r", ch );
		return;
	}
	if( IS_CLASS( victim, CLASS_VAMPIRE ) )
	{
		send_to_char( "You can't hatch a dragon from a Vampire.\n\r", ch );
		return;
	}
	if( IS_CLASS( victim, CLASS_DROW ) )
	{
		send_to_char( "You can't hatch a dragon from a Drow.\n\r", ch );
		return;
	}
	if( IS_CLASS( victim, CLASS_NINJA ) )
	{
		send_to_char( "You can't hatch a dragon from a Ninja.\n\r", ch );
		return;
	}
	if( IS_CLASS( victim, CLASS_MONK ) )
	{
		send_to_char( "You can't hatch a dragon from a Monk.\n\r", ch );
		return;
	}

    act("You consume $N's essence and hatch a new egg!", ch, NULL, victim, TO_CHAR);
    act("$n consume $N's essence and hatch a new egg!", ch, NULL, victim, TO_NOTVICT);
    act("$n consumes your essence and makes you a dragon!", ch, NULL, victim, TO_VICT);

    victim->class=0;
    victim->class=CLASS_DRAGON;
	victim->pcdata->dragonage = 1;
    victim->special=0;
	victim->level = LEVEL_AVATAR;
    victim->class = CLASS_DRAGON;

    ch->pcdata->hatch += 1;
    save_char_obj(ch);
    save_char_obj(victim);
    return;
}

const struct dgain_type dgain_table[] =
{
    { "Wings",           DRA_WINGS,            0,            2500  },
    { "Fangs",           DRA_FANGS,            0,            2500  },
    { "Tail",            DRA_TAIL,             VERY_YOUNG,   2500  },     
    { "Scry",            DRA_SCRY,             0,            2500  },
    { "Peer",            DRA_PEER,             0,            2500  },
    { "Truesight",       DRA_TRUESIGHT,        0,            2500  },  
    { "Lick",            DRA_LICK,             VERY_YOUNG,   5000  },
    { "Fly",             DRA_FLY,              ADULT,        5000  }, 
    { "Terror",	         DRA_TERROR,	       0,            7500  },
    { "Speed",           DRA_CONFUSE,          0,            7500  },
    { "Mask",            DRA_MASK,             OLD,          7500  },
    { "Magic",           DRA_MAGIC,            0,            7500  },
    { "Polymorph",       DRA_POLYMORPH,        YOUNG_ADULT,  10000 },
    { "Luck",            DRA_LUCK,             ADULT,        10000 },
    { "Toughness",       DRA_TUFF,             GREAT_WYRM,   10000 },
    { "Frostbreath",     DRA_FROST_BREATH,     JUVENILE,     50000 },  
    { "Firebreath",      DRA_FIRE_BREATH,      VENERABLE,    50000 },   
    { "Lightningbreath", DRA_LIGHTNING_BREATH, MATURE_ADULT, 50000 },       
    { "Acidbreath",      DRA_ACID_BREATH,      GREAT_WYRM,   50000 },           
    { "Gasbreath",       DRA_GAS_BREATH,       0,            50000 },    
    { NULL,              -1,                   YOUNG,        0 }
};

void do_dgain( CHAR_DATA *ch, char *argument )
{
    char      arg1 [MAX_INPUT_LENGTH];
//    char      arg2 [MAX_INPUT_LENGTH];
    int       inpart = 0;
    int       cost = 0;

    smash_tilde(argument);
    argument = one_argument( argument, arg1 );
//    argument = one_argument( argument, arg2 );

    if (IS_NPC(ch)) return;

	if ( !IS_CLASS(ch, CLASS_DRAGON) )
	{
	    send_to_char("Huh?\n\r", ch);
	    return;
	}

    if ( arg1[0] == '\0' )
    {
stc("{D-=-=-=-=-=-=-=-=-=-=-=-=-=-{W   Dragon    {D-=-=-=-=-=-=-=-=-=-=-=-=-=-{x\n\r",ch);
stc("{m       Wings {r[{w 2500{r]{m       Fangs {r[{w 2500{r]{m      Scry {r[{w 2500{r]{x\n\r",ch);
stc("{m        Peer {r[{w 2500{r]{m   Truesight {r[{w 2500{r]{m    Terror {r[{w 7500{r]{x\n\r",ch);
stc("{m       Speed {r[{w10000{r]{m       Magic {r[{w 7500{r]{m Gasbreath {r[{w50000{r]{x\n\r",ch);
stc("{m        Tail {r[{w 2500{r]{m                          Lick {r[{w 5000{r]{x\n\r",ch);
stc("{m                            Mask {r[{w 7500{r]{x\n\r",ch);
stc("{m   Polymorph {r[{w10000{r]                    {m Toughness {r[{w10000{r]{x\n\r",ch);
stc("{m        Fly  {r[{w 5000{r]                      {m    Luck {r[{w10000{r]{x\n\r",ch);
stc("{D-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n\r",ch);
stc("{C              Frostbreath {r[{w50000{r]{R  Firebreath {r[{w50000{r]{x\n\r",ch);
stc("{Y          Lightningbreath {r[{w50000{r]{G  Acidbreath {r[{w50000{r]{x\n\r",ch);
stc("{D-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n\r",ch);
	return;
    }


    if (!str_cmp(arg1,"wings")) 
	{inpart = DRA_WINGS; cost = 2500;}
    else if (!str_cmp(arg1,"fangs"))
        {inpart = DRA_FANGS; cost = 2500;}
    else if (!str_cmp(arg1,"tail"))
	{inpart = DRA_TAIL; cost = 2500;}
    else if (!str_cmp(arg1,"scry"))
	{inpart = DRA_SCRY; cost = 2500;}
    else if (!str_cmp(arg1,"peer"))
	{inpart = DRA_PEER; cost = 2500;}
    else if (!str_cmp(arg1,"truesight"))
	{inpart = DRA_TRUESIGHT; cost = 2500;}
    else if (!str_cmp(arg1,"lick"))
	{inpart = DRA_LICK; cost = 5000;}
    else if (!str_cmp(arg1,"fly")) 
	{inpart = DRA_FLY; cost = 5000;}
    else if (!str_cmp(arg1,"terror")) 
	{inpart = DRA_TERROR; cost = 7500;}
   else if (!str_cmp(arg1, "speed"))
	{inpart = DRA_CONFUSE; cost = 10000;} 
   else if (!str_cmp(arg1,"mask")) 
	{inpart = DRA_MASK; cost = 7500;}
    else if (!str_cmp(arg1,"magic")) 
	{inpart = DRA_MAGIC; cost = 7500;}
    else if (!str_cmp(arg1,"polymorph")) 
	{inpart = DRA_POLYMORPH; cost = 10000;}
    else if (!str_cmp(arg1,"luck")) 
	{inpart = DRA_LUCK; cost = 10000;}
    else if (!str_cmp(arg1,"toughness")) 
	{inpart = DRA_TUFF; cost = 10000;}
    else if (!str_cmp(arg1,"frostbreath")) 
	{inpart = DRA_FROST_BREATH; cost = 50000;}
    else if (!str_cmp(arg1,"firebreath")) 
	{inpart = DRA_FIRE_BREATH; cost = 50000;}
    else if (!str_cmp(arg1,"lightningbreath")) 
	{inpart = DRA_LIGHTNING_BREATH; cost = 50000;}
    else if (!str_cmp(arg1,"acidbreath"))
	{inpart = DRA_ACID_BREATH; cost = 50000;}
    else if (!str_cmp(arg1,"gasbreath")) 
	{inpart = DRA_GAS_BREATH; cost = 50000;}
    else
    {
	do_dgain(ch,"");
	return;
    }
    if (IS_DRAPOWER(ch, inpart))
    {
	send_to_char("You already have that power.\n\r",ch);
	return;
    }
    if (ch->pcdata->stats[DEMON_CURRENT] < cost)
    {
	send_to_char("You don't have enough dragon points to gain that power.\n\r",ch);
	return;
    }
    SET_BIT(ch->pcdata->dragonaff, inpart);
    ch->pcdata->stats[DEMON_CURRENT] -= cost;
    send_to_char("Ok.\n\r",ch);
    do_autosave(ch,"");
    return;
}

void do_dgain2( CHAR_DATA *ch, char *argument )
{
	char arg[MAX_INPUT_LENGTH];
	char buf[MAX_INPUT_LENGTH];
	int count = 1;

    argument = one_argument( argument, arg );

     if (!IS_CLASS(ch, CLASS_DRAGON))
     {
       stc("You're not a dragon!\n\r",ch);
       return;
     }

	if(IS_SET(ch->pcdata->dragonaff, DRA_WINGS )) count += 1;
	if(IS_SET(ch->pcdata->dragonaff, DRA_FANGS )) count += 1;
	if(IS_SET(ch->pcdata->dragonaff, DRA_LICK )) count += 1;
	if(IS_SET(ch->pcdata->dragonaff, DRA_FROST_BREATH )) count += 1;
	if(IS_SET(ch->pcdata->dragonaff, DRA_FIRE_BREATH )) count += 1;
	if(IS_SET(ch->pcdata->dragonaff, DRA_LIGHTNING_BREATH )) count += 1;
	if(IS_SET(ch->pcdata->dragonaff, DRA_ACID_BREATH )) count += 1;
	if(IS_SET(ch->pcdata->dragonaff, DRA_GAS_BREATH )) count += 1;
	if(IS_SET(ch->pcdata->dragonaff, DRA_TAIL )) count += 1;
	if(IS_SET(ch->pcdata->dragonaff, DRA_TERROR )) count += 1;
	if(IS_SET(ch->pcdata->dragonaff, DRA_CONFUSE )) count += 1;
	if(IS_SET(ch->pcdata->dragonaff, DRA_MASK )) count += 1;
	if(IS_SET(ch->pcdata->dragonaff, DRA_MAGIC )) count += 1;
	if(IS_SET(ch->pcdata->dragonaff, DRA_POLYMORPH )) count += 1;
	if(IS_SET(ch->pcdata->dragonaff, DRA_LUCK )) count += 1;
	if(IS_SET(ch->pcdata->dragonaff, DRA_FLY )) count += 1;
	if(IS_SET(ch->pcdata->dragonaff, DRA_SCRY )) count += 1;
	if(IS_SET(ch->pcdata->dragonaff, DRA_PEER )) count += 1;
	if(IS_SET(ch->pcdata->dragonaff, DRA_TRUESIGHT )) count += 1;
	if(IS_SET(ch->pcdata->dragonaff, DRA_TOUGHNESS)) count += 1;

	if( IS_NPC(ch) ) return;

	if( !IS_IMMORTAL(ch))
	{
	if( !IS_CLASS(ch, CLASS_DRAGON))
	{
		send_to_char( "Huh?!?\n\r", ch );
		return;
	}
	}

	if( arg[0] == '\0' )
	{
		int x = 0;
		send_to_char( "Valid choices are:  ALL(shows skills learned)\n\r", ch );
		if( !IS_SET(ch->pcdata->dragonaff, DRA_WINGS ) && ch->pcdata->dragonage >= YOUNG )
		{
			send_to_char( "Wings,", ch );
			x++;
			if( x > 5 ){send_to_char( "\n\r", ch );x = 0;}
		}
		if( !IS_SET(ch->pcdata->dragonaff, DRA_SCRY ) )
		{
			send_to_char( " Scry,", ch );
			x++;
			if( x > 5){send_to_char("\n\r", ch);x = 0;}
		}
		if( !IS_SET(ch->pcdata->dragonaff, DRA_TRUESIGHT ) )
		{
			send_to_char(" Truesight,", ch );
			x++;
			if(x>5){send_to_char("\n\r", ch);x=0;}
		}
		if( !IS_SET(ch->pcdata->dragonaff, DRA_PEER ) )
		{
			send_to_char( " Peer,", ch );
			x++;
			if(x > 5){send_to_char("\n\r", ch);x=0;}
		}
		if( !IS_SET(ch->pcdata->dragonaff, DRA_FANGS ) )
		{
			send_to_char( " Fangs,", ch );
			x++;
			if( x > 5 ){send_to_char( "\n\r", ch );x = 0;}
		}
		if( !IS_SET(ch->pcdata->dragonaff, DRA_LICK ) )
		{
			send_to_char( " Lick,", ch );
			x++;
			if( x > 5 ){send_to_char( "\n\r", ch );x = 0;}
		}
		if( !IS_SET(ch->pcdata->dragonaff, DRA_FROST_BREATH ) && ch->pcdata->dragonage >= JUVENILE)
		{
			send_to_char( " FrostBreath,", ch );
			x++;
			if( x > 5 ){send_to_char( "\n\r", ch );x = 0;}
		}
		if( !IS_SET(ch->pcdata->dragonaff, DRA_FIRE_BREATH ) && ch->pcdata->dragonage >= VENERABLE )
		{
			send_to_char( " FireBreath,", ch );
			x++;
			if( x > 5 ){send_to_char( "\n\r", ch );x = 0;}
		}
		if( !IS_SET(ch->pcdata->dragonaff, DRA_LIGHTNING_BREATH ) && ch->pcdata->dragonage >= MATURE_ADULT )
		{
			send_to_char( " LightningBreath,", ch );
			x++;
			if( x > 5 ){send_to_char( "\n\r", ch );x = 0;}
		}
		if( !IS_SET(ch->pcdata->dragonaff, DRA_ACID_BREATH ) && ch->pcdata->dragonage >= GREAT_WYRM )
		{
			send_to_char( " AcidBreath,", ch );
			x++;
			if( x > 5 ){send_to_char( "\n\r", ch );x = 0;}
		}
		if( !IS_SET(ch->pcdata->dragonaff, DRA_GAS_BREATH ) )
		{
			send_to_char( " GasBreath,", ch );
			x++;
			if( x > 5 ){send_to_char( "\n\r", ch );x = 0;}
		}
		if( !IS_SET(ch->pcdata->dragonaff, DRA_TAIL ) && ch->pcdata->dragonage >= VERY_YOUNG )
		{
			send_to_char( " Tail,", ch );
			x++;
			if( x > 5 ){send_to_char( "\n\r", ch );x = 0;}
		}
		if( !IS_SET(ch->pcdata->dragonaff, DRA_TERROR ) )
		{
			send_to_char( " Terror,", ch );
			x++;
			if( x > 5 ){send_to_char( "\n\r", ch );x = 0;}
		}
		if( !IS_SET(ch->pcdata->dragonaff, DRA_CONFUSE ) )
		{
			send_to_char( " Confuse,", ch );
			x++;
			if( x > 5 ){send_to_char( "\n\r", ch );x = 0;}
		}
		if( !IS_SET(ch->pcdata->dragonaff, DRA_MASK ) && ch->pcdata->dragonage >= OLD )
		{
			send_to_char( " Mask,", ch );
			x++;
			if( x > 5 ){send_to_char( "\n\r", ch );x = 0;}
		}
		if( !IS_SET(ch->pcdata->dragonaff, DRA_MAGIC ) )
		{
			send_to_char( " Magic,", ch );
			x++;
			if( x > 5 ){send_to_char( "\n\r", ch );x = 0;}
		}
		if( !IS_SET(ch->pcdata->dragonaff, DRA_POLYMORPH ) && ch->pcdata->dragonage >= YOUNG_ADULT )
		{
			send_to_char( " Polymorph,", ch );
			x++;
			if( x > 5 ){send_to_char( "\n\r", ch );x = 0;}
		}
		if( !IS_SET(ch->pcdata->dragonaff, DRA_LUCK ) && ch->pcdata->dragonage >= ADULT )
		{
			send_to_char( " Luck", ch );
			x++;
			if( x > 5 ){send_to_char( "\n\r", ch );x = 0;}
		}
		if( !IS_SET(ch->pcdata->dragonaff, DRA_FLY	) && ch->pcdata->dragonage >= ADULT )
		{
			send_to_char( " Fly,", ch );
			x++;
			if( x > 5 ){send_to_char( "\n\r", ch );x = 0;}
		}
		if( !IS_SET(ch->pcdata->dragonaff, DRA_TOUGHNESS) && ch->pcdata->dragonage >= GREAT_WYRM)
		{
			send_to_char( " Scales,", ch );
			x++;
			if( x > 5 ){send_to_char( "\n\r", ch );x = 0;}
		}
		send_to_char( "\n\r", ch );
		send_to_char( "Some are only avaiable after a certain age. Check helps for ages.\n\r", ch );
		return;
	}
	else if( !str_cmp( arg, "all"	) )
	{
		int x = 0;
		send_to_char( "You have the folowing dragon powers:\n\r", ch );
		if(IS_SET(ch->pcdata->dragonaff, DRA_WINGS )) 
		{
			send_to_char( "Wings,", ch );
			x++;
			if( x > 5 ){send_to_char( "\n\r", ch );x = 0;}
		}
		if( IS_SET(ch->pcdata->dragonaff, DRA_FLY	) )
		{
			send_to_char( " Fly,", ch );
			x++;
			if( x > 5 ){send_to_char( "\n\r", ch );x = 0;}
		}
		if( IS_SET(ch->pcdata->dragonaff, DRA_TRUESIGHT ) )
		{
			send_to_char(" Truesight,", ch );
			x++;
			if(x>5){send_to_char("\n\r", ch);x=0;}
		}
		if( IS_SET(ch->pcdata->dragonaff, DRA_PEER ) )
		{
			send_to_char( " Peer,", ch );
			x++;
			if(x > 5){send_to_char("\n\r", ch);x=0;}
		}
		if(IS_SET(ch->pcdata->dragonaff, DRA_FANGS )) 
		{
			send_to_char( " Fangs,", ch );
			x++;
			if( x > 5 ){send_to_char( "\n\r", ch );x = 0;}
		}
		if( IS_SET(ch->pcdata->dragonaff, DRA_SCRY ) )
		{
			send_to_char( " Scry,", ch );
			x++;
			if( x > 5){send_to_char("\n\r", ch);x = 0;}
		}
		if(IS_SET(ch->pcdata->dragonaff, DRA_LICK )) 
		{
			send_to_char( " Lick,", ch );
			x++;
			if( x > 5 ){send_to_char( "\n\r", ch );x = 0;}
		}
		if(IS_SET(ch->pcdata->dragonaff, DRA_FROST_BREATH )) 
		{
			send_to_char( " FrostBreath,", ch );
			x++;
			if( x > 5 ){send_to_char( "\n\r", ch );x = 0;}
		}
		if(IS_SET(ch->pcdata->dragonaff, DRA_FIRE_BREATH )) 
		{
			send_to_char( " FireBreath,", ch );
			x++;
			if( x > 5 ){send_to_char( "\n\r", ch );x = 0;}
		}
		if(IS_SET(ch->pcdata->dragonaff, DRA_LIGHTNING_BREATH )) 
		{
			send_to_char( " LightningBreath,", ch );
			x++;
			if( x > 5 ){send_to_char( "\n\r", ch );x = 0;}
		}
		if(IS_SET(ch->pcdata->dragonaff, DRA_ACID_BREATH )) 
		{
			send_to_char( " AcidBreath,", ch );
			x++;
			if( x > 5 ){send_to_char( "\n\r", ch );x = 0;}
		}
		if(IS_SET(ch->pcdata->dragonaff, DRA_GAS_BREATH )) 
		{
			send_to_char( " GasBreath,", ch );
			x++;
			if( x > 5 ){send_to_char( "\n\r", ch );x = 0;}
		}
		if(IS_SET(ch->pcdata->dragonaff, DRA_TAIL )) 
		{
			send_to_char( " Tail,", ch );
			x++;
			if( x > 5 ){send_to_char( "\n\r", ch );x = 0;}
		}
		if(IS_SET(ch->pcdata->dragonaff, DRA_TERROR )) 
		{
			send_to_char( " Terror,", ch );
			x++;
			if( x > 5 ){send_to_char( "\n\r", ch );x = 0;}
		}
		if(IS_SET(ch->pcdata->dragonaff, DRA_CONFUSE )) 
		{
			send_to_char( " Confuse,", ch );
			x++;
			if( x > 5 ){send_to_char( "\n\r", ch );x = 0;}
		}
		if(IS_SET(ch->pcdata->dragonaff, DRA_MASK )) 
		{
			send_to_char( " Mask,", ch );
			x++;
			if( x > 5 ){send_to_char( "\n\r", ch );x = 0;}
		}
		if(IS_SET(ch->pcdata->dragonaff, DRA_MAGIC )) 
		{
			send_to_char( " Magic,", ch );
			x++;
			if( x > 5 ){send_to_char( "\n\r", ch );x = 0;}
		}
		if(IS_SET(ch->pcdata->dragonaff, DRA_POLYMORPH )) 
		{
			send_to_char( " Polymorph,", ch );
			x++;
			if( x > 5 ){send_to_char( "\n\r", ch );x = 0;}
		}
		if(IS_SET(ch->pcdata->dragonaff, DRA_LUCK ))
		{
			send_to_char( " Luck", ch );
			x++;
			if( x > 5 ){send_to_char( "\n\r", ch );x = 0;}
		}
		if( !IS_SET(ch->pcdata->dragonaff, DRA_TOUGHNESS))
		{
			send_to_char( " Scales,", ch );
			x++;
			if( x > 5 ){send_to_char( "\n\r", ch );x = 0;}
		}
		send_to_char( "\n\r", ch );
		return;
	}
	else if( !str_cmp( arg, "wings" ) )
	{
		if( IS_SET(ch->pcdata->dragonaff, DRA_WINGS ) )
		{
			send_to_char( "You already have your wings.\n\r", ch );
			return;
		}
		if( ch->pcdata->dragonage < YOUNG )
		{
			send_to_char( "You must be a young dragon to learn this power.\n\r", ch );
			return;
		}
		if( ch->exp < count * 50000 )
		{
                        sprintf( buf, "You need %d exp.\n\r", count * 50000 );
			return;
		}
		ch->exp -= count * 50000;
		send_to_char( "You now have your wings.\n\r", ch );
		SET_BIT( ch->pcdata->dragonaff, DRA_WINGS );
		return;
	}
	else if( !str_cmp( arg, "sight" ) )
	{
		if( IS_SET(ch->pcdata->dragonaff, DRA_TRUESIGHT ))
		{
			send_to_char( "You already have sight beyond sight.\n\r", ch );
			return;
		}
		if( ch->exp < count * 50000 )
		{
                       sprintf( buf, "You need %d exp.\n\r", count * 50000 );                  
			return;
		}
		ch->exp -= count * 50000;
		send_to_char( "You now have sight beyond sight.\n\r", ch );
		SET_BIT( ch->pcdata->dragonaff, DRA_TRUESIGHT );
		return;
	}
	else if( !str_cmp( arg, "fangs" ) )
	{
		if( IS_SET(ch->pcdata->dragonaff, DRA_FANGS ))
		{
			send_to_char( "You already have your fangs.\n\r", ch );
			return;
		}
		if( ch->exp < count * 50000 )
		{
                       sprintf( buf, "You need %d exp.\n\r", count * 50000 );                  
			send_to_char( buf, ch );
			return;
		}
		ch->exp -= count * 50000;
		send_to_char( "You now have your fangs.\n\r", ch );
		SET_BIT( ch->pcdata->dragonaff, DRA_FANGS );
		return;
	}
	else if( !str_cmp( arg, "lick" ))
	{
		if( IS_SET(ch->pcdata->dragonaff, DRA_LICK ))
		{
			send_to_char( "You already can can heal your wounds by licking 'em.\n\r", ch );
			return;
		}
		if( ch->exp < count * 50000 )
		{
                       sprintf( buf, "You need %d exp.\n\r", count * 50000 );                  
			send_to_char( buf, ch );
			return;
		}
		ch->exp -= count * 50000;
		send_to_char( "You now can heal your wounds by licking them.\n\r", ch );
		SET_BIT( ch->pcdata->dragonaff, DRA_LICK );
		return;
	}
	else if( !str_cmp( arg, "frostbreath" ))
	{
		if( IS_SET(ch->pcdata->dragonaff, DRA_FROST_BREATH ))
		{
			send_to_char( "You already can breath frost.\n\r", ch );
			return;
		}
		if( ch->pcdata->dragonage < JUVENILE )
		{
			send_to_char( "You must be a juvenile dragon to learn this skill.\n\r", ch );
			return;
		}
		if( ch->exp < count * 50000 )
		{
                       sprintf( buf, "You need %d exp.\n\r", count * 50000 );                  
			send_to_char( buf, ch );
			return;
		}
		ch->exp -= count * 50000;
		send_to_char( "You now can breath frost.\n\r", ch );
		SET_BIT( ch->pcdata->dragonaff, DRA_FROST_BREATH );
		return;
	}
	else if( !str_cmp( arg, "firebreath" ))
	{
		if( IS_SET(ch->pcdata->dragonaff, DRA_FIRE_BREATH ))
		{
			send_to_char( "You already can breath fire.\n\r", ch );
			return;
		}
		if( ch->pcdata->dragonage < VENERABLE )
		{
			send_to_char( "You must be a venerable dragon to learn this power.\n\r", ch );
			return;
		}
		if( ch->exp < count * 50000 )
		{
                       sprintf( buf, "You need %d exp.\n\r", count * 50000 );                  
			send_to_char( buf, ch );
			return;
		}
		ch->exp -= count * 50000;
		send_to_char( "You now can breath fire.\n\r", ch );
		SET_BIT( ch->pcdata->dragonaff, DRA_FIRE_BREATH );
		return;
	}
	else if( !str_cmp( arg, "lightningbreath" ))
	{
		if( IS_SET(ch->pcdata->dragonaff, DRA_LIGHTNING_BREATH ))
		{
			send_to_char( "You already can breath lightning.\n\r", ch );
			return;
		}
		if( ch->pcdata->dragonage < MATURE_ADULT )
		{
			send_to_char( "You mus be a mature dragon adult to learn this power.\n\r", ch );
			return;
		}
		if( ch->exp < count * 50000 )
		{
                       sprintf( buf, "You need %d exp.\n\r", count * 50000 );                  
			send_to_char( buf, ch );
			return;
		}
		ch->exp -= count * 50000;
		send_to_char( "You now can breath lightning.\n\r", ch );
		SET_BIT( ch->pcdata->dragonaff, DRA_LIGHTNING_BREATH );
		return;
	}
	else if( !str_cmp( arg, "acidbreath" ))
	{
		if( IS_SET(ch->pcdata->dragonaff, DRA_ACID_BREATH ))
		{
			send_to_char( "You already can breath acid.\n\r", ch );
			return;
		}
		if( ch->pcdata->dragonage < GREAT_WYRM )
		{
			send_to_char( "You mus be a great wyrm to learn this power.\n\r", ch );
			return;
		}
		if( ch->exp < count * 50000 )
		{
                       sprintf( buf, "You need %d exp.\n\r", count * 50000 );                  
			send_to_char( buf, ch );
			return;
		}
		ch->exp -= count * 50000;
		send_to_char( "You now can breath acid.\n\r", ch );
		SET_BIT( ch->pcdata->dragonaff, DRA_ACID_BREATH );
		return;
	}
	else if( !str_cmp( arg, "gasbreath" ))
	{
		if( IS_SET(ch->pcdata->dragonaff, DRA_GAS_BREATH ))
		{
			send_to_char( "You already can breath gas.\n\r", ch );
			return;
		}
		if( ch->exp < count * 50000 )
		{
                       sprintf( buf, "You need %d exp.\n\r", count * 50000 );                  
			send_to_char( buf, ch );
			return;
		}
		ch->exp -= count * 50000;
		send_to_char( "You now can breath gas.\n\r", ch );
		SET_BIT( ch->pcdata->dragonaff, DRA_GAS_BREATH );
		return;
	}
	else if( !str_cmp( arg, "tail" ))
	{
		if( IS_SET(ch->pcdata->dragonaff, DRA_TAIL ))
		{
			send_to_char( "You already can use your tail to attack.\n\r", ch );
			return;
		}
		if( ch->pcdata->dragonage < VERY_YOUNG )
		{
			send_to_char( "You must be a very young dragon to learn this power.\n\r", ch );
			return;
		}
		if( ch->exp < count * 50000 )
		{
                       sprintf( buf, "You need %d exp.\n\r", count * 50000 );                  
			send_to_char( buf, ch );
			return;
		}
		ch->exp -= count * 50000;
		send_to_char( "You now can use your tail to attack.\n\r", ch );
		SET_BIT( ch->pcdata->dragonaff, DRA_TAIL );
		return;
	}
	else if( !str_cmp( arg, "terror" ))
	{
		if( IS_SET(ch->pcdata->dragonaff, DRA_TERROR ))
		{
			send_to_char( "You already inspire terror in mobs.\n\r", ch );
			return;
		}
		if( ch->exp < count * 50000 )
		{
                       sprintf( buf, "You need %d exp.\n\r", count * 50000 );                  
			send_to_char( buf, ch );
			return;
		}
		ch->exp -= count * 50000;
		send_to_char( "You now inspire terror in mobs.\n\r", ch );
		SET_BIT( ch->pcdata->dragonaff, DRA_TERROR );
		return;
	}
	else if( !str_cmp( arg, "confuse" ))
	{
		if( IS_SET(ch->pcdata->dragonaff, DRA_CONFUSE ))
		{
			send_to_char( "You already can confuse your targets.\n\r", ch );
			return;
		}
		if( ch->exp < count * 50000 )
		{
                       sprintf( buf, "You need %d exp.\n\r", count * 50000 );                  
			send_to_char( buf, ch );
			return;
		}
		ch->exp -= count * 50000;
		send_to_char( "You now can confuse your targets.\n\r", ch );
		SET_BIT( ch->pcdata->dragonaff, DRA_CONFUSE );
		return;
	}
	else if( !str_cmp( arg, "mask" ))
	{
		if( IS_SET(ch->pcdata->dragonaff, DRA_MASK ))
		{
			send_to_char( "You already can mask people.\n\r", ch );
			return;
		}
		if( ch->pcdata->dragonage < OLD )
		{
			send_to_char( "You must be an old dragon to learn this power.\n\r", ch );
			return;
		}
		if( ch->exp < count * 50000 )
		{
                       sprintf( buf, "You need %d exp.\n\r", count * 50000 );                  
			send_to_char( buf, ch );
			return;
		}
		ch->exp -= count * 50000;
		send_to_char( "You now can mask other people.\n\r", ch );
		SET_BIT( ch->pcdata->dragonaff, DRA_MASK );
		return;
	}
	else if( !str_cmp( arg, "scry" ))
	{
		if( IS_SET(ch->pcdata->dragonaff, DRA_SCRY))
		{
			send_to_char( "You already can see people who are far away.\n\r", ch);
			return;
		}
		if( ch->exp < count * 50000 )
		{
                       sprintf( buf, "You need %d exp.\n\r", count * 50000 );                  
			send_to_char( buf, ch );
			return;
		}
		ch->exp -= count * 50000;
		send_to_char( "You can now see people who are far away.\n\r", ch );
		SET_BIT( ch->pcdata->dragonaff, DRA_SCRY );
		return;
	}
	else if( !str_cmp( arg, "polymorph" ))
	{
		if( IS_SET(ch->pcdata->dragonaff, DRA_POLYMORPH ))
		{
			send_to_char( "You already can polymorph.\n\r", ch );
			return;
		}
		if( ch->pcdata->dragonage < YOUNG_ADULT )
		{
			send_to_char( "You must be a young adult dragon to learn this power.\n\r", ch );
			return;
		}
		if( ch->exp < count * 50000 )
		{
                       sprintf( buf, "You need %d exp.\n\r", count * 50000 );                  
			send_to_char( buf, ch );
			return;
		}
		ch->exp -= count * 50000;
		send_to_char( "You now can polymorph.\n\r", ch );
		SET_BIT( ch->pcdata->dragonaff, DRA_POLYMORPH );
		return;
	} 
	else if( !str_cmp( arg, "luck" ))
	{
		if( IS_SET(ch->pcdata->dragonaff, DRA_LUCK ))
		{
			send_to_char( "You already have extra luck.\n\r", ch );
			return;
		}
		if( ch->pcdata->dragonage < ADULT )
		{
			send_to_char( "You have to be at least an adult dragon to get this power.\n\r", ch );
			return;
		}
		if( ch->exp < count * 50000 )
		{
                       sprintf( buf, "You need %d exp.\n\r", count * 50000 );                  
			send_to_char( buf, ch );
			return;
		}
		ch->exp -= count * 50000;
		send_to_char( "You now have extra luck.\n\r", ch );
		SET_BIT( ch->pcdata->dragonaff, DRA_LUCK );
		return;
	}
	else if( !str_cmp( arg, "fly"	))
	{
		if( IS_SET(ch->pcdata->dragonaff, DRA_FLY ))
		{
			send_to_char( "You already can fly to targets.\n\r", ch );
			return;
		}
		if( ch->pcdata->dragonage < ADULT )
		{
			send_to_char( "You have to be at least an adult dragon to get this power.\n\r", ch );
			return;
		}
		if( ch->exp < count * 50000 )
		{
                       sprintf( buf, "You need %d exp.\n\r", count * 50000 );                  
			send_to_char( buf, ch );
			return;
		}
		ch->exp -= count * 50000;
		send_to_char( "You now can fly to people.\n\r", ch );
		SET_BIT( ch->pcdata->dragonaff, DRA_FLY );
		return;
	}
	else if( !str_cmp( arg, "scales"))
	{
		if( IS_SET(ch->pcdata->dragonaff, DRA_TOUGHNESS ))
		{
			send_to_char( "Your scales are already very tough.\n\r", ch );
			return;
		}
		if( ch->pcdata->dragonage < GREAT_WYRM )
		{
			send_to_char( "You have to be at least a great wyrm to get this power.\n\r", ch);
			return;
		}
		if( ch->exp < count * 50000 )
		{
                       sprintf( buf, "You need %d exp.\n\r", count * 50000 );                  
			send_to_char( buf, ch );
			return;
		}
		ch->exp -= count * 50000;
		send_to_char( "Your scales now are tougher than ever.\n\r", ch );
		SET_BIT( ch->pcdata->dragonaff, DRA_TOUGHNESS );
		return;
	}
	else if( !str_cmp( arg, "peer"	))
	{
		if( IS_SET(ch->pcdata->dragonaff, DRA_PEER	))
		{
			send_to_char( "You already can see people's stats.\n\r", ch );
			return;
		}
		if( ch->exp < count * 50000 )
		{
                       sprintf( buf, "You need %d exp.\n\r", count * 50000 );                  
			send_to_char( buf, ch );
			return;
		}
		ch->exp -= count * 50000;
		SET_BIT(ch->pcdata->dragonaff, DRA_PEER);
		send_to_char( "You now can see other people's stats.\n\r", ch );
		return;
	}
	else if( !str_cmp( arg, "magic" ))
	{
		if( IS_SET(ch->pcdata->dragonaff, DRA_MAGIC ))
		{
			send_to_char( "You already have magic control.\n\r", ch );
			return;
		}
		if( ch->exp < count * 50000 )
		{
                       sprintf( buf, "You need %d exp.\n\r", count * 50000 );                  
			send_to_char( buf, ch );
			return;
		}
		ch->exp -= count * 50000;
		send_to_char( "You now have great magical control.\n\r", ch );
		SET_BIT( ch->pcdata->dragonaff, DRA_MAGIC);
		return;
	}
	else
	{
		int x = 0;
		send_to_char( "Valid choices are:\n\r", ch );
		if( !IS_SET(ch->pcdata->dragonaff, DRA_WINGS ) && ch->pcdata->dragonage >= YOUNG )
		{
			send_to_char( "Wings,", ch );
			x++;
			if( x > 5 ){send_to_char( "\n\r", ch );x = 0;}
		}
		if( !IS_SET(ch->pcdata->dragonaff, DRA_FLY	) && ch->pcdata->dragonage >= ADULT )
		{
			send_to_char( " Fly,", ch );
			x++;
			if( x > 5 ){send_to_char( "\n\r", ch );x = 0;}
		}
		if( !IS_SET(ch->pcdata->dragonaff, DRA_TRUESIGHT ) )
		{
			send_to_char(" Truesight,", ch );
			x++;
			if(x>5){send_to_char("\n\r", ch);x=0;}
		}
		if( !IS_SET(ch->pcdata->dragonaff, DRA_SCRY ) )
		{
			send_to_char( " Scry,", ch );
			x++;
			if( x > 5){send_to_char("\n\r", ch);x = 0;}
		}
		if( !IS_SET(ch->pcdata->dragonaff, DRA_FANGS ) )
		{
			send_to_char( " Fangs,", ch );
			x++;
			if( x > 5 ){send_to_char( "\n\r", ch );x = 0;}
		}
		if( !IS_SET(ch->pcdata->dragonaff, DRA_LICK ) )
		{
			send_to_char( " Lick,", ch );
			x++;
			if( x > 5 ){send_to_char( "\n\r", ch );x = 0;}
		}
		if( !IS_SET(ch->pcdata->dragonaff, DRA_FROST_BREATH ) && ch->pcdata->dragonage >= GREAT_WYRM)
		{
			send_to_char( " FrostBreath,", ch );
			x++;
			if( x > 5 ){send_to_char( "\n\r", ch );x = 0;}
		}
		if( !IS_SET(ch->pcdata->dragonaff, DRA_FIRE_BREATH ) && ch->pcdata->dragonage >= VENERABLE )
		{
			send_to_char( " FireBreath,", ch );
			x++;
			if( x > 5 ){send_to_char( "\n\r", ch );x = 0;}
		}
		if( !IS_SET(ch->pcdata->dragonaff, DRA_LIGHTNING_BREATH ) && ch->pcdata->dragonage >= MATURE_ADULT )
		{
			send_to_char( " LightningBreath,", ch );
			x++;
			if( x > 5 ){send_to_char( "\n\r", ch );x = 0;}
		}
		if( !IS_SET(ch->pcdata->dragonaff, DRA_ACID_BREATH ) && ch->pcdata->dragonage >= JUVENILE )
		{
			send_to_char( " AcidBreath,", ch );
			x++;
			if( x > 5 ){send_to_char( "\n\r", ch );x = 0;}
		}
		if( !IS_SET(ch->pcdata->dragonaff, DRA_GAS_BREATH ) )
		{
			send_to_char( " Gas Breath,", ch );
			x++;
			if( x > 5 ){send_to_char( "\n\r", ch );x = 0;}
		}
		if( !IS_SET(ch->pcdata->dragonaff, DRA_PEER ) )
		{
			send_to_char( " Peer,", ch );
			x++;
			if(x > 5){send_to_char("\n\r", ch);x=0;}
		}
		if( !IS_SET(ch->pcdata->dragonaff, DRA_TAIL ) && ch->pcdata->dragonage >= VERY_YOUNG )
		{
			send_to_char( " Tail,", ch );
			x++;
			if( x > 5 ){send_to_char( "\n\r", ch );x = 0;}
		}
		if( !IS_SET(ch->pcdata->dragonaff, DRA_TERROR ) )
		{
			send_to_char( " Terror,", ch );
			x++;
			if( x > 5 ){send_to_char( "\n\r", ch );x = 0;}
		}
		if( !IS_SET(ch->pcdata->dragonaff, DRA_CONFUSE ) )
		{
			send_to_char( " Confuse,", ch );
			x++;
			if( x > 5 ){send_to_char( "\n\r", ch );x = 0;}
		}
		if( !IS_SET(ch->pcdata->dragonaff, DRA_MASK ) && ch->pcdata->dragonage >= OLD )
		{
			send_to_char( "Mask,", ch );
			x++;
			if( x > 5 ){send_to_char( "\n\r", ch );x = 0;}
		}
		if( !IS_SET(ch->pcdata->dragonaff, DRA_MAGIC ) )
		{
			send_to_char( " Magic,", ch );
			x++;
			if( x > 5 ){send_to_char( "\n\r", ch );x = 0;}
		}
		if( !IS_SET(ch->pcdata->dragonaff, DRA_POLYMORPH ) && ch->pcdata->dragonage >= YOUNG_ADULT )
		{
			send_to_char( " Polymorph,", ch );
			x++;
			if( x > 5 ){send_to_char( "\n\r", ch );x = 0;}
		}
		if( !IS_SET(ch->pcdata->dragonaff, DRA_LUCK ) && ch->pcdata->dragonage >= ADULT )
		{
			send_to_char( " Luck,", ch );
			x++;
			if( x > 5 ){send_to_char( "\n\r", ch );x = 0;}
		}
		if( !IS_SET(ch->pcdata->dragonaff, DRA_TOUGHNESS) && ch->pcdata->dragonage >= GREAT_WYRM)
		{
			send_to_char( " Scales,", ch );
			x++;
			if( x > 5 ){send_to_char( "\n\r", ch );x = 0;}
		}
		send_to_char( "\n\r", ch );
		send_to_char( "Some are only avaiable after a certain age. Check helps for ages.\n\r", ch );
		return;
	}

	return;
}

void do_ddbreath( CHAR_DATA *ch, char *argument )
{
	char arg1[MAX_INPUT_LENGTH];
	char arg2[MAX_INPUT_LENGTH];
	char arg3[MAX_INPUT_LENGTH];
	char arg4[MAX_INPUT_LENGTH];
	char arg5[MAX_INPUT_LENGTH];
	char buf[MAX_INPUT_LENGTH];

	if( IS_NPC(ch) ) return;
	buf[0] = '\0';

	argument = one_argument( argument, arg1 );
	argument = one_argument( argument, arg2 );
	argument = one_argument( argument, arg3 );
	argument = one_argument( argument, arg4 );
	argument = one_argument( argument, arg5 );

	if( !IS_IMMORTAL(ch))
	{
	if( !IS_CLASS(ch, CLASS_DRAGON) )
	{
		send_to_char( "Huh?!?\n\r", ch );
		return;
	}
	}

	if( arg1[0] == '\0' )
	{
		send_to_char( "You can breath the following types:\n\r", ch );
		if( IS_SET(ch->pcdata->dragonaff, DRA_GAS_BREATH))
			strcat( buf, "gas " );
		if( IS_SET(ch->pcdata->dragonaff, DRA_LIGHTNING_BREATH))
			strcat( buf, "lightning " );
		if( IS_SET(ch->pcdata->dragonaff, DRA_FROST_BREATH))
			strcat( buf, "frost " );
		if( IS_SET(ch->pcdata->dragonaff, DRA_FIRE_BREATH))
			strcat( buf, "fire " );
		if( IS_SET(ch->pcdata->dragonaff, DRA_ACID_BREATH))
			strcat( buf, "acid" );
		strcat( buf, "\n\r" );
		send_to_char( buf, ch );
		return;
	}
	else if( !str_cmp( arg1, "gas" ) && IS_SET(ch->pcdata->dragonaff, DRA_GAS_BREATH ))
	{
		free_string( ch->pcdata->breath[0] );
		ch->pcdata->breath[0] = str_dup("gas");
		send_to_char( "You now breath gas in battle.\n\r", ch );
	}
	else if( !str_cmp( arg1, "lightning" ) && IS_SET(ch->pcdata->dragonaff, DRA_LIGHTNING_BREATH))
	{
		free_string( ch->pcdata->breath[0] );
		ch->pcdata->breath[0] = str_dup("lightning");
		send_to_char( "You now breath lightning in battle.\n\r", ch );
	}
	else if( !str_cmp( arg1, "frost" ) && IS_SET(ch->pcdata->dragonaff, DRA_FROST_BREATH))
	{
		free_string( ch->pcdata->breath[0] );
		ch->pcdata->breath[0] = str_dup("frost");
		send_to_char( "You now breath frost in battle.\n\r", ch );
	}
	else if( !str_cmp( arg1, "fire" ) && IS_SET(ch->pcdata->dragonaff, DRA_FIRE_BREATH))
	{
		free_string( ch->pcdata->breath[0] );
		ch->pcdata->breath[0] = str_dup("fire");
		send_to_char( "You now breath fire in battle.\n\r", ch );
	}
	else if( !str_cmp( arg1, "acid" ) && IS_SET(ch->pcdata->dragonaff, DRA_ACID_BREATH))
	{
		free_string( ch->pcdata->breath[0] );
		ch->pcdata->breath[0] = str_dup("acid");
		send_to_char( "You now breath acid in battle.\n\r", ch );
	}
	else
	{
		send_to_char( "You can breath the following types:\n\r", ch );
		buf[0] = '\0';
		if( IS_SET(ch->pcdata->dragonaff, DRA_GAS_BREATH))
			strcat( buf, "gas " );
		if( IS_SET(ch->pcdata->dragonaff, DRA_LIGHTNING_BREATH))
			strcat( buf, "lightning " );
		if( IS_SET(ch->pcdata->dragonaff, DRA_FROST_BREATH))
			strcat( buf, "frost " );
		if( IS_SET(ch->pcdata->dragonaff, DRA_FIRE_BREATH))
			strcat( buf, "fire " );
		if( IS_SET(ch->pcdata->dragonaff, DRA_ACID_BREATH))
			strcat( buf, "acid" );
		strcat( buf, "\n\r" );
		send_to_char( buf, ch );
		return;
	}
	if( arg2[0] == '\0')
	{
	    free_string( ch->pcdata->breath[1] );
	    free_string( ch->pcdata->breath[2] );
	    free_string( ch->pcdata->breath[3] );
	    free_string( ch->pcdata->breath[4] );
	    ch->pcdata->breath[1] = str_dup("");
	    ch->pcdata->breath[2] = str_dup("");
	    ch->pcdata->breath[3] = str_dup("");
	    ch->pcdata->breath[4] = str_dup("");
	    return;
	}
	else if( !str_cmp( arg2, "gas" ) && IS_SET(ch->pcdata->dragonaff, DRA_GAS_BREATH ))
	{
		free_string( ch->pcdata->breath[1] );
		ch->pcdata->breath[1] = str_dup("gas");
		send_to_char( "You now breath gas in battle.\n\r", ch );
	}
	else if( !str_cmp( arg2, "lightning" ) && IS_SET(ch->pcdata->dragonaff, DRA_LIGHTNING_BREATH))
	{
		free_string( ch->pcdata->breath[1] );
		ch->pcdata->breath[1] = str_dup("lightning");
		send_to_char( "You now breath lightning in battle.\n\r", ch );
	}
	else if( !str_cmp( arg2, "frost" ) && IS_SET(ch->pcdata->dragonaff, DRA_FROST_BREATH))
	{
		free_string( ch->pcdata->breath[1] );
		ch->pcdata->breath[1] = str_dup("frost");
		send_to_char( "You now breath frost in battle.\n\r", ch );
	}
	else if( !str_cmp( arg2, "fire" ) && IS_SET(ch->pcdata->dragonaff, DRA_FIRE_BREATH))
	{
		free_string( ch->pcdata->breath[1] );
		ch->pcdata->breath[1] = str_dup("fire");
		send_to_char( "You now breath fire in battle.\n\r", ch );
	}
	else if( !str_cmp( arg2, "acid" ) && IS_SET(ch->pcdata->dragonaff, DRA_ACID_BREATH))
	{
		free_string( ch->pcdata->breath[1] );
		ch->pcdata->breath[1] = str_dup("acid");
		send_to_char( "You now breath acid in battle.\n\r", ch );
	}
	else
	{
		send_to_char( "You can breath the following types:\n\r", ch );
		buf[0] = '\0';
		if( IS_SET(ch->pcdata->dragonaff, DRA_GAS_BREATH))
			strcat( buf, "gas " );
		if( IS_SET(ch->pcdata->dragonaff, DRA_LIGHTNING_BREATH))
			strcat( buf, "lightning " );
		if( IS_SET(ch->pcdata->dragonaff, DRA_FROST_BREATH))
			strcat( buf, "frost " );
		if( IS_SET(ch->pcdata->dragonaff, DRA_FIRE_BREATH))
			strcat( buf, "fire " );
		if( IS_SET(ch->pcdata->dragonaff, DRA_ACID_BREATH))
			strcat( buf, "acid" );
		strcat( buf, "\n\r" );
		send_to_char( buf, ch );
		return;
	}
	if( arg3[0] == '\0')
	{
	    free_string( ch->pcdata->breath[2] );
	    free_string( ch->pcdata->breath[3] );
	    free_string( ch->pcdata->breath[4] );
	    ch->pcdata->breath[2] = str_dup("");
	    ch->pcdata->breath[3] = str_dup("");
	    ch->pcdata->breath[4] = str_dup("");
	    return;
	}
	else if( !str_cmp( arg3, "gas" ) && IS_SET(ch->pcdata->dragonaff, DRA_GAS_BREATH ))
	{
		free_string( ch->pcdata->breath[2] );
		ch->pcdata->breath[2] = str_dup("gas");
		send_to_char( "You now breath gas in battle.\n\r", ch );
	}
	else if( !str_cmp( arg3, "lightning" ) && IS_SET(ch->pcdata->dragonaff, DRA_LIGHTNING_BREATH))
	{
		free_string( ch->pcdata->breath[2] );
		ch->pcdata->breath[2] = str_dup("lightning");
		send_to_char( "You now breath lightning in battle.\n\r", ch );
	}
	else if( !str_cmp( arg3, "frost" ) && IS_SET(ch->pcdata->dragonaff, DRA_FROST_BREATH))
	{
		free_string( ch->pcdata->breath[2] );
		ch->pcdata->breath[2] = str_dup("frost");
		send_to_char( "You now breath frost in battle.\n\r", ch );
	}
	else if( !str_cmp( arg3, "fire" ) && IS_SET(ch->pcdata->dragonaff, DRA_FIRE_BREATH))
	{
		free_string( ch->pcdata->breath[2] );
		ch->pcdata->breath[2] = str_dup("fire");
		send_to_char( "You now breath fire in battle.\n\r", ch );
	}
	else if( !str_cmp( arg3, "acid" ) && IS_SET(ch->pcdata->dragonaff, DRA_ACID_BREATH))
	{
		free_string( ch->pcdata->breath[2] );
		ch->pcdata->breath[2] = str_dup("acid");
		send_to_char( "You now breath acid in battle.\n\r", ch );
	}
	else
	{
		send_to_char( "You can breath the following types:\n\r", ch );
		buf[0] = '\0';
		if( IS_SET(ch->pcdata->dragonaff, DRA_GAS_BREATH))
			strcat( buf, "gas " );
		if( IS_SET(ch->pcdata->dragonaff, DRA_LIGHTNING_BREATH))
			strcat( buf, "lightning " );
		if( IS_SET(ch->pcdata->dragonaff, DRA_FROST_BREATH))
			strcat( buf, "frost " );
		if( IS_SET(ch->pcdata->dragonaff, DRA_FIRE_BREATH))
			strcat( buf, "fire " );
		if( IS_SET(ch->pcdata->dragonaff, DRA_ACID_BREATH))
			strcat( buf, "acid" );
		strcat( buf, "\n\r" );
		send_to_char( buf, ch );
		return;
	}
	if( arg4[0] == '\0')
	{
	    free_string( ch->pcdata->breath[3] );
	    free_string( ch->pcdata->breath[4] );
	    ch->pcdata->breath[3] = str_dup("");
	    ch->pcdata->breath[4] = str_dup("");
	    return;
	}
	else if( !str_cmp( arg4, "gas" ) && IS_SET(ch->pcdata->dragonaff, DRA_GAS_BREATH ))
	{
		free_string( ch->pcdata->breath[3] );
		ch->pcdata->breath[3] = str_dup("gas");
		send_to_char( "You now breath gas in battle.\n\r", ch );
	}
	else if( !str_cmp( arg4, "lightning" ) && IS_SET(ch->pcdata->dragonaff, DRA_LIGHTNING_BREATH))
	{
		free_string( ch->pcdata->breath[3] );
		ch->pcdata->breath[3] = str_dup("lightning");
		send_to_char( "You now breath lightning in battle.\n\r", ch );
	}
	else if( !str_cmp( arg4, "frost" ) && IS_SET(ch->pcdata->dragonaff, DRA_FROST_BREATH))
	{
		free_string( ch->pcdata->breath[3] );
		ch->pcdata->breath[3] = str_dup("frost");
		send_to_char( "You now breath frost in battle.\n\r", ch );
	}
	else if( !str_cmp( arg4, "fire" ) && IS_SET(ch->pcdata->dragonaff, DRA_FIRE_BREATH))
	{
		free_string( ch->pcdata->breath[3] );
		ch->pcdata->breath[3] = str_dup("fire");
		send_to_char( "You now breath fire in battle.\n\r", ch );
	}
	else if( !str_cmp( arg4, "acid" ) && IS_SET(ch->pcdata->dragonaff, DRA_ACID_BREATH))
	{
		free_string( ch->pcdata->breath[3] );
		ch->pcdata->breath[3] = str_dup("acid");
		send_to_char( "You now breath acid in battle.\n\r", ch );
	}
	else
	{
		send_to_char( "You can breath the following types:\n\r", ch );
		buf[0] = '\0';
		if( IS_SET(ch->pcdata->dragonaff, DRA_GAS_BREATH))
			strcat( buf, "gas " );
		if( IS_SET(ch->pcdata->dragonaff, DRA_LIGHTNING_BREATH))
			strcat( buf, "lightning " );
		if( IS_SET(ch->pcdata->dragonaff, DRA_FROST_BREATH))
			strcat( buf, "frost " );
		if( IS_SET(ch->pcdata->dragonaff, DRA_FIRE_BREATH))
			strcat( buf, "fire " );
		if( IS_SET(ch->pcdata->dragonaff, DRA_ACID_BREATH))
			strcat( buf, "acid" );
		strcat( buf, "\n\r" );
		send_to_char( buf, ch );
		return;
	}
	if( arg5[0] == '\0')
	{
	    free_string( ch->pcdata->breath[4] );
	    ch->pcdata->breath[4] = str_dup("");
	    return;
	}
	else if( !str_cmp( arg5, "gas" ) && IS_SET(ch->pcdata->dragonaff, DRA_GAS_BREATH ))
	{
		free_string( ch->pcdata->breath[4] );
		ch->pcdata->breath[4] = str_dup("gas");
		send_to_char( "You now breath gas in battle.\n\r", ch );
	}
	else if( !str_cmp( arg5, "lightning" ) && IS_SET(ch->pcdata->dragonaff, DRA_LIGHTNING_BREATH))
	{
		free_string( ch->pcdata->breath[4] );
		ch->pcdata->breath[4] = str_dup("lightning");
		send_to_char( "You now breath lightning in battle.\n\r", ch );
	}
	else if( !str_cmp( arg5, "frost" ) && IS_SET(ch->pcdata->dragonaff, DRA_FROST_BREATH))
	{
		free_string( ch->pcdata->breath[4] );
		ch->pcdata->breath[4] = str_dup("frost");
		send_to_char( "You now breath frost in battle.\n\r", ch );
	}
	else if( !str_cmp( arg5, "fire" ) && IS_SET(ch->pcdata->dragonaff, DRA_FIRE_BREATH))
	{
		free_string( ch->pcdata->breath[4] );
		ch->pcdata->breath[4] = str_dup("fire");
		send_to_char( "You now breath fire in battle.\n\r", ch );
	}
	else if( !str_cmp( arg5, "acid" ) && IS_SET(ch->pcdata->dragonaff, DRA_ACID_BREATH))
	{
		free_string( ch->pcdata->breath[4] );
		ch->pcdata->breath[4] = str_dup("acid");
		send_to_char( "You now breath acid in battle.\n\r", ch );
	}
	else
	{
		send_to_char( "You can breath the following types:\n\r", ch );
		buf[0] = '\0';
		if( IS_SET(ch->pcdata->dragonaff, DRA_GAS_BREATH))
			strcat( buf, "gas " );
		if( IS_SET(ch->pcdata->dragonaff, DRA_LIGHTNING_BREATH))
			strcat( buf, "lightning " );
		if( IS_SET(ch->pcdata->dragonaff, DRA_FROST_BREATH))
			strcat( buf, "frost " );
		if( IS_SET(ch->pcdata->dragonaff, DRA_FIRE_BREATH))
			strcat( buf, "fire " );
		if( IS_SET(ch->pcdata->dragonaff, DRA_ACID_BREATH))
			strcat( buf, "acid" );
		strcat( buf, "\n\r" );
		send_to_char( buf, ch );
		return;
	}
}

void do_lick( CHAR_DATA *ch, char *argument )
{
	char arg[MAX_INPUT_LENGTH];
	CHAR_DATA *victim;

	one_argument(argument, arg );

	if( !IS_IMMORTAL(ch) )
	{
	if( !IS_CLASS(ch, CLASS_DRAGON) )
	{
		send_to_char( "Huh?!?\n\r", ch );
		return;
	}
	if( !IS_SET(ch->pcdata->dragonaff, DRA_LICK) )
	{
		send_to_char( "You haven't learned this skill yet.\n\r", ch );
		return;
	}
	}

/*    if( ch->pcdata->condition[COND_FULL] < 30 )
    {
	stc( "You need to eat more before you can use this.\n\r",ch);
	return;
    }*/

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
    send_to_char( "They aren't here.\n\r", ch );
    return;
    }
	if( arg[0] == '\0' )
		victim = ch;

	victim->hit += 100 * ch->pcdata->dragonage;
	victim->move += 100 * ch->pcdata->dragonage;	
	if( victim->hit > victim->max_hit )
		victim->hit = victim->max_hit;
	if( victim->move > victim->max_move )
		victim->move = victim->max_move;

	update_pos(victim);

	act( "You lick $N's wounds and heal $M.",ch,NULL,victim,TO_CHAR);
	act( "$n licks your wounds and heals $M.",ch,NULL,victim,TO_VICT);
	act( "$n licks $N's wounds and heals $M.",ch,NULL,victim,TO_NOTVICT);
    gain_condition( ch, COND_FULL, -(number_range(10,30)));
    WAIT_STATE(ch,9);
	return;
}

void do_ddconfuse( CHAR_DATA *ch, char *argument )
{
	CHAR_DATA *victim;
	char arg[MAX_INPUT_LENGTH];
        char buf[MAX_INPUT_LENGTH];
    AFFECT_DATA af;

	one_argument( argument, arg );

	if( !IS_IMMORTAL(ch))
	{
	if( !IS_CLASS(ch, CLASS_DRAGON) )
	{
		send_to_char( "Huh?!?\n\r", ch );
		return;
	}
	if( !IS_SET(ch->pcdata->dragonaff, DRA_CONFUSE) )
	{
		send_to_char( "You haven't learned this skill yet.\n\r", ch );
		return;
	}
	}


	if( arg[0] == '\0' )
	{
		send_to_char( "Confuse whom?\n\r", ch );
		return;
	}

	if( (victim = get_char_room(ch, arg)) == NULL )
	{
		send_to_char( "They aren't here.\n\r", ch );
		return;
	}

        if ( is_affected( victim, gsn_curse )) { stc("They are already cursed!\n\r",ch); return; }
if (is_safe(ch,victim) ) return;

    af.type      = gsn_curse;
    af.duration  = 5;
    af.location  = APPLY_HITROLL;
    af.modifier  = -200;
    af.bitvector = 0;
    affect_to_char( victim, &af );

    af.location  = APPLY_DAMROLL;
    af.modifier  = -200;
    affect_to_char( victim, &af );
    gain_condition( ch, COND_FULL, -(number_range(40,50)));
    WAIT_STATE(ch,12);

    stc("You curse them!\n\r",ch);
    sprintf(buf,"%s curses you!\n\r",ch->name);
    stc(buf,victim);
	return;
}

void do_dpolymorph( CHAR_DATA *ch, char *argument )
{
	char arg[MAX_INPUT_LENGTH];
    /*AFFECT_DATA af;*/
    char buf [MAX_INPUT_LENGTH];
	OBJ_DATA *obj;

	one_argument( argument, arg );

	if( !IS_IMMORTAL(ch))
	{
	if( !IS_CLASS(ch, CLASS_DRAGON) )
	{
		send_to_char( "Huh?!?\n\r", ch);
		return;
	}
	if( !IS_SET(ch->pcdata->dragonaff, DRA_POLYMORPH) )
	{
		send_to_char( "You haven't learned this skill yet.\n\r", ch );
		return;
	}
	}

	if( arg[0] == '\0' )
	{
		send_to_char( "You can polymorph into the following forms:\n\r", ch );
		send_to_char( "Normal, Frog, Fish, Raven, Lizard, Stone.\n\r", ch );
		return;
	}
	else if( !str_cmp( arg, "normal" ) )
	{
		act("$n changes back to normal.",ch,NULL,NULL,TO_ROOM);
		send_to_char( "You polymorph back to normal.\n\r", ch );
		REMOVE_BIT(ch->affected_by, AFF_POLYMORPH);
		REMOVE_BIT(ch->extra,EXTRA_OSWITCH);
		free_string(ch->morph);
	    ch->morph = str_dup("");
	}
    else if ( !str_cmp( arg, "frog" ) )
    {
	if (!IS_NPC(ch) && ch->stance[0] != -1) do_stance(ch,"");
	send_to_char("You polymorph into a frog!\n\r",ch);
	sprintf(buf,"%s the frog",ch->name);
    SET_BIT(ch->affected_by, AFF_POLYMORPH);
	free_string(ch->morph);
	ch->morph = str_dup(buf);
	return;
    }
    else if ( !str_cmp( arg, "fish" ) )
    {
	act("$n polymorphs into a fish!",ch,NULL,NULL,TO_ROOM);
	send_to_char("You polymorph into a fish!\n\r",ch);
    SET_BIT(ch->affected_by, AFF_POLYMORPH);
	sprintf(buf,"%s the fish",ch->name);
	free_string(ch->morph);
	ch->morph = str_dup(buf);
	return;
    }
    else if ( !str_cmp( arg, "raven" ) )
    {
	act("$n polymorphs into a raven!",ch,NULL,NULL,TO_ROOM);
	send_to_char("You polymorph into a raven!\n\r",ch);
    SET_BIT(ch->affected_by, AFF_POLYMORPH);
	sprintf(buf,"%s the raven",ch->name);
	free_string(ch->morph);
	ch->morph = str_dup(buf);
	return;
    }
	else if ( !str_cmp(arg, "lizard" ) )
	{
		act("$n polymorphs into a lizard!",ch,NULL,NULL,TO_ROOM);
		send_to_char("You polymorph into a lizard!\n\r",ch);
		SET_BIT(ch->affected_by, AFF_POLYMORPH);
		sprintf(buf,"%s the lizard",ch->name);
		free_string(ch->morph);
		ch->morph = str_dup(buf);
		return;
	}
	else if ( !str_cmp(arg, "stone" ) )
	{
    if ((obj = create_object(get_obj_index(5234),60)) == NULL)
    {
	send_to_char( "You don't have the ability to change into a weapon.\n\r", ch);
	return;
    }
    obj_to_room(obj,ch->in_room);
    act("$n transforms into $p and falls to the ground.",ch,obj,NULL,TO_ROOM);
    act("You transform into $p and fall to the ground.",ch,obj,NULL,TO_CHAR);
    ch->pcdata->obj_vnum = 5234;
    obj->chobj = ch;
    ch->pcdata->chobj = obj;
    SET_BIT(ch->affected_by, AFF_POLYMORPH);
    SET_BIT(ch->extra, EXTRA_OSWITCH);
    free_string(ch->morph);
    ch->morph = str_dup(obj->short_descr);
    return;
	}
	else
	{
		send_to_char( "You can polymorph into the following forms:\n\r", ch );
		send_to_char( "Normal, Frog, Fish, Raven, Lizard, Stone.\n\r", ch );
		return;
	}
	return;		
}

void do_fly( CHAR_DATA *ch, char *argument )
{
        CHAR_DATA *victim;
        char      arg[MAX_INPUT_LENGTH];
    
        argument = one_argument(argument, arg);
    
        if( IS_NPC(ch) ) return;
        if( !IS_CLASS(ch, CLASS_DRAGON) )
        {
                send_to_char("Huh?", ch);
                return;
        }
     
        if (!IS_DRAPOWER(ch, DRA_FLY))
        { stc("Try learning it first.\n\r",ch); return; }

        if (ch->fight_timer > 0)
        {
        send_to_char("Not until your fight timer expires.\n\r", ch );
        return;
        }
    
        if( arg[0] == '\0' )
        {
                send_to_char("Fly to whom?\r\n", ch);
                return;
        }
                
    if ( ( victim = get_char_world( ch, arg ) ) == NULL
    ||   victim == ch
    ||   victim->in_room == NULL
    ||   ch->in_room == NULL)
    {
                send_to_char( "You attempt to fly to them, but fail.\n\r", ch );
                return;
    }

    if ( victim->in_room == ch->in_room )
    {
                send_to_char( "But you are already there!\n\r", ch );
                return;
    }
         
    if ( IS_SET(victim->in_room->room_flags, ROOM_SAFE)
    ||   IS_SET(victim->in_room->room_flags, ROOM_PRIVATE)
    ||   IS_SET(victim->in_room->room_flags, ROOM_SOLITARY)
    ||   IS_SET(victim->in_room->room_flags, ROOM_NO_RECALL)
    ||   (!IS_NPC(victim) && !IS_IMMUNE(victim, IMM_SUMMON) ) )
    {
                send_to_char( "You are unable to focus on their location.\n\r", ch );
                return;
    }
    
    send_to_char("You spread your wings and begin to fly.\n\r",ch);
    act("$n's wings spread, and $n begins to fly.",ch,NULL,NULL,TO_ROOM);
    char_from_room(ch);
    char_to_room(ch,victim->in_room);
    do_look(ch,"");
    ch->move -= (ch->max_move/10);
    if(ch->move < 0) ch->move = 0;
    WAIT_STATE(ch, 5); 
    return;
}        

void do_peer(CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *victim;
    OBJ_DATA  *obj;
    char      arg [MAX_INPUT_LENGTH];
    char      buf [MAX_INPUT_LENGTH];

    one_argument( argument, arg );

    if (IS_NPC(ch)) return;

	if(!IS_IMMORTAL(ch))
	{
	if(!IS_CLASS(ch, CLASS_DRAGON))
	{
		send_to_char( "Huh?!?\n\r", ch );
		return;
	}
	if(!IS_SET(ch->pcdata->dragonaff, DRA_PEER))
	{
		send_to_char( "You don't have the ability to know people's stats.\n\r", ch);
		return;
	}
	}

	if( arg[0] == '\0' )
	{
		send_to_char( "Peer at who?\n\r", ch );
		return;
	}
    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
    if ( ( obj = get_obj_carry( ch, arg ) ) == NULL )
    {
        send_to_char( "Peer at what?\n\r", ch );
        return;
    }
    act("$n examines $p intently.",ch,obj,NULL,TO_ROOM);
    spell_identify( skill_lookup( "identify" ), ch->level, ch, obj );
    return;
	}
    act("$n examines $N intently.",ch,NULL,victim,TO_NOTVICT);
    act("$n examines you intently.",ch,NULL,victim,TO_VICT);
    if (IS_NPC(victim)) sprintf(buf, "%s is an NPC.\n\r",victim->short_descr);
    else 
    {
    if      (victim->level == 12) sprintf(buf, "%s is an Implementor.\n\r", victim->name);
    else if (victim->level == 11) sprintf(buf, "%s is a High Judge.\n\r", victim->name);
    else if (victim->level == 10) sprintf(buf, "%s is a Judge.\n\r", victim->name);
    else if (victim->level == 9 ) sprintf(buf, "%s is an Enforcer.\n\r", victim->name);
    else if (victim->level == 8 ) sprintf(buf, "%s is a Quest Maker.\n\r", victim->name);
    else if (victim->level == 7 ) sprintf(buf, "%s is a Builder.\n\r", victim->name);
    else if (victim->level >= 3 ) sprintf(buf, "%s is an Avatar.\n\r", victim->name);
    else sprintf(buf, "%s is a Mortal.\n\r", victim->name);
    }
    send_to_char(buf,ch);

	if( !IS_NPC(victim))
	{
	if( IS_CLASS(victim, CLASS_DEMON)) sprintf( buf,"%s is a Demon.\n\r", victim->name );
	if( IS_CLASS(victim, CLASS_MAGE)) sprintf( buf,"%s is a Mage.\n\r", victim->name );
	if( IS_CLASS(victim, CLASS_WEREWOLF)) sprintf( buf,"%s is a Werewolf.\n\r", victim->name );
	if( IS_CLASS(victim, CLASS_VAMPIRE)) sprintf( buf,"%s is a Vampire.\n\r", victim->name );
	if( IS_CLASS(victim, CLASS_HIGHLANDER)) sprintf( buf,"%s is a Highlander.\n\r", victim->name );
	if( IS_CLASS(victim, CLASS_DROW)) sprintf( buf,"%s is a Drow.\n\r", victim->name );
	if( IS_CLASS(victim, CLASS_NINJA)) sprintf( buf,"%s is a Ninja.\n\r", victim->name );
	if( IS_CLASS(victim, CLASS_MONK)) sprintf( buf,"%s is a Monk.\n\r", victim->name );
	if( IS_CLASS(victim, CLASS_DRAGON)) sprintf( buf,"%s is a Dragon.\n\r", victim->name );
	}
	send_to_char( buf, ch);
    if (!IS_NPC(victim))
    {
	sprintf(buf,"Str:%d, Int:%d, Wis:%d, Dex:%d, Con:%d.\n\r",get_curr_str(victim),get_curr_int(victim),get_curr_wis(victim),get_curr_dex(victim),get_curr_con(victim));
    send_to_char(buf,ch);
    }
    
	sprintf(buf,"Hp:%ld/%ld, Mana:%ld/%ld, Move:%ld/%ld.\n\r",victim->hit,victim->max_hit,victim->mana,victim->max_mana,victim->move,victim->max_move);

    send_to_char(buf,ch);
    if (!IS_NPC(victim))
	{
		sprintf(buf,"Hitroll:%d, Damroll:%d, AC:%d.\n\r",char_hitroll(victim),char_damroll(victim),char_ac(victim));
	}
    else sprintf(buf,"AC:%d.\n\r",char_ac(victim));
    send_to_char(buf,ch);
    if (!IS_NPC(victim))
    {
    sprintf(buf,"Status:%d, ",victim->race);
    send_to_char(buf,ch);
    if (IS_CLASS(victim, CLASS_VAMPIRE))    
    {
        sprintf(buf,"Blood:%d, ",victim->pcdata->condition[COND_THIRST]);
        send_to_char(buf,ch);
    }
    }
    sprintf(buf,"Alignment:%d.\n\r",victim->alignment);
    send_to_char(buf,ch);
    if (!IS_NPC(victim) && IS_EXTRA(victim, EXTRA_PREGNANT) && victim->sex == SEX_FEMALE)
    act("$N is pregnant.",ch,NULL,victim,TO_CHAR);
    if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_VAMPIRE))    
    {
    send_to_char("Disciplines:",ch);
    if (IS_VAMPAFF(victim, VAM_PROTEAN)) send_to_char(" Protean",ch);
    if (IS_VAMPAFF(victim, VAM_VICISSITUDE)) send_to_char(" vicissitude",ch);
    if (IS_VAMPAFF(victim, VAM_CELERITY)) send_to_char(" Celerity",ch);
    if (IS_VAMPAFF(victim, VAM_FORTITUDE)) send_to_char(" Fortitude",ch);
    if (IS_VAMPAFF(victim, VAM_POTENCE)) send_to_char(" Potence",ch);
    if (IS_VAMPAFF(victim, VAM_OBFUSCATE)) send_to_char(" Obfuscate",ch);
    if (IS_VAMPAFF(victim, VAM_OBTENEBRATION)) send_to_char(" Obtenebration",ch);
    if (IS_VAMPAFF(victim, VAM_SERPENTIS)) send_to_char(" Serpentis",ch);
    if (IS_VAMPAFF(victim, VAM_AUSPEX)) send_to_char(" Auspex",ch);
    if (IS_VAMPAFF(victim, VAM_DOMINATE)) send_to_char(" Dominate",ch);
    if (IS_VAMPAFF(victim, VAM_PRESENCE)) send_to_char(" Presence",ch);
    send_to_char(".\n\r",ch);
    }


	return;
}

void do_consume( CHAR_DATA *ch, char *argument )
{
    OBJ_DATA *corpse;
    OBJ_DATA *c_next;
    OBJ_DATA *obj;
    OBJ_DATA *obj_next;
	char arg[MAX_INPUT_LENGTH];
	char buf[MAX_INPUT_LENGTH];
	int points;

	one_argument( argument, arg );

    if( !IS_CLASS(ch, CLASS_DRAGON)) return;

    for ( corpse = ch->in_room->contents; corpse != NULL; corpse = c_next )
    {
	c_next = corpse->next_content;
	if ( corpse->item_type != ITEM_CORPSE_NPC && corpse->item_type != ITEM_CORPSE_PC)
	    continue;
	points = number_range( 10, 70 * (ch->race+1) );
	ch->pcdata->stats[DEMON_CURRENT] += points;
	ch->pcdata->stats[DEMON_TOTAL] += points;
	act( "$n savagely devours a corpse.", ch, NULL, NULL, TO_ROOM );
	sprintf( buf, "You gain %d dragon points.\n\r", points );
	send_to_char( buf, ch );
	for ( obj = corpse->contains; obj; obj = obj_next )
	{
	    obj_next = obj->next_content;
	    obj_from_obj( obj );
	    obj_to_room( obj, ch->in_room );
	}

	if (corpse != NULL) extract_obj( corpse );
    }
	return;
}

void do_dgem ( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    OBJ_INDEX_DATA *pObjIndex;
    OBJ_DATA *obj;
    int value;

    argument = one_argument( argument, arg1 );
 
    if( !IS_CLASS(ch, CLASS_DRAGON) )
    {
	stc("Huh?\n\r", ch);
	return;
    }

    if ( IS_NPC(ch) || (ch->pcdata->stats[DEMON_CURRENT] < 1 ) )
    {
	send_to_char("You are unable to make a gem.\n\r",ch);
	return;
    }
    else if ( arg1[0] == '\0' || !is_number( arg1 ) )
    {
	send_to_char("Please specify a value for gem.\n\r",ch);
	return;
    }
    else
    {
        value = atoi( arg1 );
	if ( value < 1 || value > 100 )
        {
	    send_to_char( "Gem value should have a value between 1 and 100.\n\r", ch );
	    return;
        }
	else if ( value > ch->pcdata->stats[DEMON_CURRENT] )
        {
	    sprintf(buf,"You only have %d quest points left to put into tokens.\n\r",ch->pcdata->quest);
	    send_to_char( buf, ch );
	    return;
        }
    }

    if ( ( pObjIndex = get_obj_index( OBJ_VNUM_PROTOPLASM ) ) == NULL )
    {
	send_to_char( "Error...missing object, please inform KaVir.\n\r", ch );
	return;
    }

    ch->pcdata->stats[DEMON_CURRENT] -= value;
	ch->pcdata->stats[DEMON_TOTAL]	 -= value;
    if (ch->pcdata->stats[DEMON_CURRENT]  < 0) ch->pcdata->stats[DEMON_CURRENT] = 0;
    obj = create_object( pObjIndex, value );
    obj->value[0] = value;
    obj->level = 1;
    obj->cost = value*1000;
    obj->item_type = ITEM_DRAGONGEM;
    obj_to_char(obj,ch);
    if (obj->questmaker != NULL) free_string(obj->questmaker);
    obj->questmaker = str_dup(ch->name);
    free_string( obj->name );
    obj->name = str_dup( "dragon gem" );
    sprintf(buf,"a %d point gem",value);
    free_string( obj->short_descr );
    obj->short_descr = str_dup( buf );
    sprintf(buf,"A %d point gem lies on the floor.",value);
    free_string( obj->description );
    obj->description = str_dup( buf );

   	act( "You snap your fingers and reveal $p.", ch, obj, NULL, TO_CHAR );
   	act( "$n snaps $s fingers and reveals $p.", ch, obj, NULL, TO_ROOM );

    return;
}

/*void dragon_sleep( CHAR_DATA *ch )
{
	OBJ_DATA *obj;
	OBJ_DATA *obj_next;
	
	if( ch->tick_timer[TIMER_DSLEEP] != 0 ) return;
	for ( obj = ch->carrying; obj != NULL; obj = obj_next )
	{
	    obj_next = obj->next_content;

		if( obj->item_type == ITEM_DRAGONGEM ) break;
	}
	if( obj != NULL ) {do_eat( ch, obj->name );return;}

	do_sleep( ch, "" );

	send_to_char( "You fall into a deep sleep since you became so hungry.\n\r", ch );
	SET_TIMER(ch, TIMER_DSLEEP, 5);

	return;
}*/

void do_beastie( CHAR_DATA *ch, char *argument)
{
    if( IS_NPC(ch) ) return;
    if( ch->sex != SEX_FEMALE){stc("Huh?\n\r",ch);return;}

/*    if( IS_SET(ch->flag2, AFF2_BEASTIE)
    {
	SET_BIT(ch->flag2, AFF2_BEASTIE);
	act( " ",ch, NULL, NULL, TO_CHAR);
	act( " ",ch, NULL, NULL, TO_ROOM);
    }
    else
    {*/
	SET_BIT(ch->flag2, AFF2_BEASTIE);
	act( "Your eyes widen in pain and pleasure as the stalion thrusts forward stretching you beyond belife.\n\rYou feel the stalions head swell with in you several pleasure filled minutes later before a warm blast\n\rof cum shoots into you hard enough to make you orgasm anew.\n\r",ch, NULL, NULL, TO_CHAR);
	act( "You watch as $n's pussy is stretched beyond belife by a stalions huge cock.\n\rAfter minutes of watching that huge cock pentrate $n you here her gasp loudy and watch as she shakes violently. \n\rWhen the stalion pulls out you see some of his cum trickle from her stretched cunt.\n\r",ch, NULL, NULL, TO_ROOM);
    return;
}

void do_push( CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *victim;
    char arg[MAX_STRING_LENGTH];
    bool target = TRUE;

    one_argument(argument,arg);

    if( IS_NPC(ch)) return;

    if( ch->sex != SEX_FEMALE) return;

    if( !IS_SET(ch->flag2,AFF2_BEASTIE)) return;

    if ( ( victim = get_char_room( ch, arg ) ) == NULL ) target = FALSE;

    if( target)
    {
    if( IS_NPC(victim))
    {
	stc( "Not on mobs.\n\r",ch);
	return;
    }
    }

    if ( !target )
    {
	act( "You spread your legs and push on your belly with the palms of your hands.\n\r The horses cum comes outly slowly at first and then in a rush that sends you into an orgasm as the hot cum leaves your cunt and runs down your legs.\n\r",ch,NULL,NULL,TO_CHAR);
	act( "You watch as $n places her hands on her stomach and pushes slowly. \n\r You watch the first trickles run out and then gasp in suprise as it gushes out of her cunt and runs down her legs to pool at her feet.\n\r",ch,NULL,NULL,TO_ROOM);
	REMOVE_BIT(ch->flag2, AFF2_BEASTIE);
    }
    else if(victim->sex == SEX_MALE)
    {
	act( "You stradle $N and put your hands to your belly and push feeling the first drops fall to hit $N. \n\r You here $N gasp as the hot cum inside you gushes out of your cunt and into him.\n\r",ch,NULL,victim,TO_CHAR);
	act( "$n stradles you and you gaze up at her cunt as she pushes on her stomach and the first drop of hot horse sperm fall from her pussy to you.\n\r As you gaze up at $n's pussy dripping cum onto you suddenly it is pouring out onto you soaking you in horse cum.\n\r",ch,NULL,victim,TO_VICT);
	act( "$n stands over $N one leg on each side and you gaze at her cunt while she pushes on her stomach, forceing some of the cum within to drip out.\n\r Your eyes are glued to $n as her pussy suddenly spasms and cum pours out onto $N.\n\r",ch,NULL,victim,TO_NOTVICT);
	REMOVE_BIT(ch->flag2, AFF2_BEASTIE);
    }
    else
    {
	act( "You stradle $N and press on your stomach causeing cum to drip out onto her face and breast.\n\r As you push harder you feel the stalions cum begin to run out of you and onto $N's face  and breast soaking her.\n\r",ch,NULL,victim,TO_CHAR);
	act( "You look up as $n stands one leg on either side of you and begins to force the cum from her body, slowly dripping on you.\n\r You gasp in pleasure as $n's pussy opens and hot horse cum runs out onto your body and into your mouth  letting you taste the mixed juices.\n\r",ch,NULL,victim,TO_VICT);
	act( "As you watch $n stands over $N forceing the cum in her body to drip out onto $N.\n\r You lick your lips as you see the hot cum suddenly rush out onto a supprised $N.\n\r",ch,NULL,victim,TO_NOTVICT);
	REMOVE_BIT(ch->flag2, AFF2_BEASTIE);
    }
    return;
}
void do_dragonarmor( CHAR_DATA *ch, char *argument )
{
    OBJ_INDEX_DATA *pObjIndex;
    OBJ_DATA *obj;
    char arg[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    char buf2[MAX_STRING_LENGTH];
    char ooga[MAX_STRING_LENGTH];
    char ooga2[MAX_STRING_LENGTH];
    char ooga3[MAX_STRING_LENGTH];
    int vnum = 29614;
    int value = 0;
    int spell = 0;    
    argument = one_argument( argument, arg );
    
    if (IS_NPC(ch)) return;
    
    if (!IS_CLASS(ch, CLASS_DRAGON))
    {
      send_to_char("Huh?\n\r",ch);
      return;
    }

    if (IS_NPC(ch))
    {
            send_to_char("Huh?\n\r",ch);
            return;
    }
    
    if (arg[0] == '\0')
    {   
        send_to_char("Please specify what kind of equipment you want to create.\n\r", ch );
        send_to_char("Cloak, Boots, Leggings, Sleeves, Belt\n\rHood, Mask, Bracer, Ring, Amulet\n\rVest, Gloves\n\r",ch);
        return;
    }
    if      (!str_cmp(arg, "cloak"   )) 
    {
    value = ITEM_WEAR_ABOUT;
    spell = 16;
    sprintf(ooga,"Cloak");
    }
    else if (!str_cmp(arg, "boots"   )) 
    {
    value = ITEM_WEAR_FEET;
      sprintf(ooga,"Boots"); 
    }
    else if (!str_cmp(arg, "leggings")) 
    {
    value = ITEM_WEAR_LEGS;
      sprintf(ooga,"Leggings");
    }
    else if (!str_cmp(arg, "sleeves" )) 
    {
    value = ITEM_WEAR_ARMS;
      sprintf(ooga,"Sleeves");
    }
    else if (!str_cmp(arg, "belt"    )) 
    {
    value = ITEM_WEAR_WAIST;
    spell = 8;
      sprintf(ooga,"Belt");
    }
    else if (!str_cmp(arg, "hood"    )) 
    {
    value = ITEM_WEAR_HEAD;
      sprintf(ooga,"Hood");
    }
    else if (!str_cmp(arg, "mask"    )) 
    {
    value = ITEM_WEAR_FACE;
    spell = 27;
      sprintf(ooga,"Mask");
    }
    else if (!str_cmp(arg, "bracer"  )) 
    {
    value = ITEM_WEAR_WRIST;
      sprintf(ooga,"Bracer");
    }
    else if (!str_cmp(arg, "ring"    )) 
    {
    value = ITEM_WEAR_FINGER;
      sprintf(ooga,"Ring");
    }
    else if (!str_cmp(arg, "amulet"  )) 
    {
    value = ITEM_WEAR_NECK;
    spell = 60;
      sprintf(ooga,"Amulet");
    }
    else if (!str_cmp(arg, "vest"    )) 
    {
    value = ITEM_WEAR_BODY;
    spell = 17;
      sprintf(ooga,"Vest");
    }
    else if (!str_cmp(arg, "gloves"  )) 
    {
    value = ITEM_WEAR_HANDS;
      sprintf(ooga,"Gloves");
    }
    else
    {
        send_to_char("That is an invalid type.\n\r", ch );
        return;
    }
    if ( ch->pcdata->stats[DEMON_CURRENT] < 5000)
    {
        send_to_char("It costs 5000 dragon points to create equipment.\n\r",ch);
        return;
    }
    if ( vnum == 0 || (pObjIndex = get_obj_index( vnum )) == NULL)
    {
        send_to_char("Missing object, please inform Tracker.\n\r",ch);
        return;
    }
    ch->pcdata->stats[DEMON_CURRENT] -= 5000;
    obj = create_object(pObjIndex, 50);
    obj_to_char(obj, ch);
    act("$p appears in your hands.",ch,obj,NULL,TO_CHAR);
    act("$p appears in $n's hands.",ch,obj,NULL,TO_ROOM);
    if (spell > 0)
    { obj->value[3] = spell; }
    obj->value[0] = 25;
    SET_BIT(obj->wear_flags, value);
//    obj->wear_flags = value + ITEM_TAKE;
    SET_BIT(obj->spectype, SITEM_DRAGON);
      sprintf(ooga2,"{gD{Gr{ga{Gg{go{Gn {gS{Gc{ga{Gl{ge{Gd{G");
      sprintf(ooga3,"dragon scaled");

      sprintf(buf,"%s {G%s{x",ooga2,ooga);
      sprintf(buf2,"%s %s",ooga3,ooga);
        free_string( obj->short_descr );
        obj->short_descr = str_dup( buf );
        free_string( obj->name );
        obj->name = str_dup( buf2 );
      sprintf(buf,"{wYou have molded it into %s %s{x\n\r",ooga2,ooga);
      stc(buf,ch);
    return;
}
/*
void do_dragonarmor(CHAR_DATA *ch, char *argument)
{

    char arg1[MAX_STRING_LENGTH];
    char arg2[MAX_STRING_LENGTH];
    OBJ_INDEX_DATA *pObjIndex;
    OBJ_DATA *obj;
    int vnum = 0;
    int cost = 0;

    argument = one_argument(argument, arg1);
    argument = one_argument(argument, arg2);

    if( !IS_CLASS(ch, CLASS_DRAGON))
    {
	stc( "Huh?\n\r",ch);
	return;
    }
    if( arg1[0] == '\0' )
    {
	stc( "Usage: dragonarmor <green, black, blue, red> <piece>\n\r",ch);
	stc( "piece = Ring, Collar, Shirt, Helmet, Leggings, Boots, Gloves, Armblades, Cape, Belt, Bracer, Mask\n\r",ch);
	stc( "green costs 5,000 dps, black = 10,000 dps, blue = 15,000 dps, red = 20,000 dps.\n\r",ch);
	return;
    }
    if( arg2[0] == '\0' )
    {
	stc( "Usage: dragonarmor <green, black, blue, red> <piece>\n\r",ch);
	stc( "piece = Ring, Collar, Shirt, Helmet, Leggings, Boots, Gloves, Armblades, Cape, Belt, Bracer, Mask\n\r",ch);
	stc( "green costs 5,000 dps, black = 10,000 dps, blue = 15,000 dps, red = 20,000 dps.\n\r",ch);
	return;
    }

    if( !str_cmp(arg1,"green") && !str_cmp(arg2,"ring"))		{cost=5000;vnum=29724;}
    else if( !str_cmp(arg1,"green") && !str_cmp(arg2,"collar"))		{cost=5000;vnum=29725;}
    else if( !str_cmp(arg1,"green") && !str_cmp(arg2,"shirt"))		{cost=5000;vnum=29726;}
    else if( !str_cmp(arg1,"green") && !str_cmp(arg2,"helmet"))		{cost=5000;vnum=29727;}
    else if( !str_cmp(arg1,"green") && !str_cmp(arg2,"leggings"))	{cost=5000;vnum=29728;}
    else if( !str_cmp(arg1,"green") && !str_cmp(arg2,"boots"))		{cost=5000;vnum=29729;}
    else if( !str_cmp(arg1,"green") && !str_cmp(arg2,"gloves"))		{cost=5000;vnum=29730;}
    else if( !str_cmp(arg1,"green") && !str_cmp(arg2,"armblades"))	{cost=5000;vnum=29731;}
    else if( !str_cmp(arg1,"green") && !str_cmp(arg2,"cape"))		{cost=5000;vnum=29732;}
    else if( !str_cmp(arg1,"green") && !str_cmp(arg2,"belt"))		{cost=5000;vnum=29733;}
    else if( !str_cmp(arg1,"green") && !str_cmp(arg2,"bracer"))		{cost=5000;vnum=29734;}
    else if( !str_cmp(arg1,"green") && !str_cmp(arg2,"mask"))		{cost=5000;vnum=29735;}

	else if( !str_cmp(arg1,"black") && !str_cmp(arg2,"ring"))	{cost=10000;vnum=29736;}
    else if( !str_cmp(arg1,"black") && !str_cmp(arg2,"collar"))		{cost=10000;vnum=29737;}
    else if( !str_cmp(arg1,"black") && !str_cmp(arg2,"shirt"))		{cost=10000;vnum=29738;}
    else if( !str_cmp(arg1,"black") && !str_cmp(arg2,"helmet"))		{cost=10000;vnum=29739;}
    else if( !str_cmp(arg1,"black") && !str_cmp(arg2,"leggings"))	{cost=10000;vnum=29740;}
    else if( !str_cmp(arg1,"black") && !str_cmp(arg2,"boots"))		{cost=10000;vnum=29741;}
    else if( !str_cmp(arg1,"black") && !str_cmp(arg2,"gloves"))		{cost=10000;vnum=29742;}
    else if( !str_cmp(arg1,"black") && !str_cmp(arg2,"armblades"))	{cost=10000;vnum=29743;}
    else if( !str_cmp(arg1,"black") && !str_cmp(arg2,"cape"))		{cost=10000;vnum=29744;}
    else if( !str_cmp(arg1,"black") && !str_cmp(arg2,"belt"))		{cost=10000;vnum=29745;}
    else if( !str_cmp(arg1,"black") && !str_cmp(arg2,"bracer"))		{cost=10000;vnum=29746;}
    else if( !str_cmp(arg1,"black") && !str_cmp(arg2,"mask"))		{cost=10000;vnum=29747;}
    
	else if( !str_cmp(arg1,"blue") && !str_cmp(arg2,"ring"))	{cost=15000;vnum=29748;}
    else if( !str_cmp(arg1,"blue") && !str_cmp(arg2,"collar"))		{cost=15000;vnum=29749;}
    else if( !str_cmp(arg1,"blue") && !str_cmp(arg2,"shirt"))		{cost=15000;vnum=29750;}
    else if( !str_cmp(arg1,"blue") && !str_cmp(arg2,"helmet"))		{cost=15000;vnum=29751;}
    else if( !str_cmp(arg1,"blue") && !str_cmp(arg2,"leggings"))	{cost=15000;vnum=29752;}
    else if( !str_cmp(arg1,"blue") && !str_cmp(arg2,"boots"))		{cost=15000;vnum=29753;}
    else if( !str_cmp(arg1,"blue") && !str_cmp(arg2,"gloves"))		{cost=15000;vnum=29754;}
    else if( !str_cmp(arg1,"blue") && !str_cmp(arg2,"armblades"))	{cost=15000;vnum=29755;}
    else if( !str_cmp(arg1,"blue") && !str_cmp(arg2,"cape"))		{cost=15000;vnum=29756;}
    else if( !str_cmp(arg1,"blue") && !str_cmp(arg2,"belt"))		{cost=15000;vnum=29757;}
    else if( !str_cmp(arg1,"blue") && !str_cmp(arg2,"bracer"))		{cost=15000;vnum=29758;}
    else if( !str_cmp(arg1,"blue") && !str_cmp(arg2,"mask"))		{cost=15000;vnum=29759;}
    
	else if( !str_cmp(arg1,"red") && !str_cmp(arg2,"ring"))		{cost=20000;vnum=29760;}
    else if( !str_cmp(arg1,"red") && !str_cmp(arg2,"collar"))		{cost=20000;vnum=29761;}
    else if( !str_cmp(arg1,"red") && !str_cmp(arg2,"shirt"))		{cost=20000;vnum=29762;}
    else if( !str_cmp(arg1,"red") && !str_cmp(arg2,"helmet"))		{cost=20000;vnum=29763;}
    else if( !str_cmp(arg1,"red") && !str_cmp(arg2,"leggings"))		{cost=20000;vnum=29764;}
    else if( !str_cmp(arg1,"red") && !str_cmp(arg2,"boots"))		{cost=20000;vnum=29765;}
    else if( !str_cmp(arg1,"red") && !str_cmp(arg2,"gloves"))		{cost=20000;vnum=29766;}
    else if( !str_cmp(arg1,"red") && !str_cmp(arg2,"armblades"))	{cost=20000;vnum=29767;}
    else if( !str_cmp(arg1,"red") && !str_cmp(arg2,"cape"))		{cost=20000;vnum=29768;}
    else if( !str_cmp(arg1,"red") && !str_cmp(arg2,"belt"))		{cost=20000;vnum=29769;}
    else if( !str_cmp(arg1,"red") && !str_cmp(arg2,"bracer"))		{cost=20000;vnum=29770;}
    else if( !str_cmp(arg1,"red") && !str_cmp(arg2,"mask"))		{cost=20000;vnum=29771;}
    else
    {
	stc( "Usage: dragonarmor <green, black, blue, red> <piece>\n\r",ch);
	stc( "piece = Ring, Collar, Shirt, Helmet, Leggings, Boots, Gloves, Armblades, Cape, Belt, Bracer, Mask\n\r",ch);
	stc( "green costs 5,000 dps, black = 10,000 dps, blue = 15,000 dps, red = 20,000 dps.\n\r",ch);
	return;
    }

    if( ch->pcdata->stats[DEMON_CURRENT] < cost )
    {
	stc( "You don't have enough dragon points.\n\r",ch);
	return;
    }
    if ( vnum == 0 || (pObjIndex = get_obj_index( vnum )) == NULL)
    {
	send_to_char("Missing object, please inform Kip.\n\r",ch);
	return;
    }
    ch->pcdata->stats[DEMON_CURRENT]	-= cost;
    ch->pcdata->stats[DEMON_TOTAL]	-= cost;
    obj = create_object(pObjIndex, 50);
    obj_to_char(obj, ch);
    act("$p appears in your hands.",ch,obj,NULL,TO_CHAR);
    act("$p appears in $n's hands.",ch,obj,NULL,TO_ROOM);

    return;
}*/
/*
void do_staredown( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char arg[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];

    argument = one_argument( argument, arg );

    if ( IS_NPC(ch) ) return;

    if ( !IS_CLASS(ch, CLASS_DRAGON) )
    {
	stc("Huh?\n\r",ch);
	return;
    }

    if ( ( victim = get_char_room( ch, arg) ) == NULL )
    {
      stc("They aren't here.\n\r",ch);
      return;
    }

    arg[0] = UPPER(arg[0]);    

    if ( ch->fighting == NULL )
    {
      stc("You must be fighting to use the gift of Staredown.\n\r",ch);
      return;
    }

    if ( victim->fighting == NULL )
    {
      sprintf(buf, "%s is not fighting anyone.", arg);
      stc(buf, ch);
      return;
    }  

    WAIT_STATE( ch, 16 );

    if ( IS_NPC(victim) )
    {
	if ( number_range( 1, 3 ) != 1 )
        {
          act("You stare deeply into $N's eyes, but nothing happens.",ch,NULL,victim,TO_CHAR);
          act("$n stares deeply into your eyes.",ch,NULL,victim,TO_VICT);
          act("$n stares deeply into $N's eyes, but nothing happens.",ch,NULL,victim,TO_NOTVICT);
	    return;
        }

      act("You stare into $N's eyes, and $E flees in terror.",ch,NULL,victim,TO_CHAR);
      act("$n stares deeply into your eyes, and you are filled with terror.",ch,NULL,victim,TO_VICT);
      act("$n stares deeply into $N's eyes, and $E flees in terror.",ch,NULL,victim,TO_NOTVICT);
      do_flee( victim, "" );
      return;
    }
    else
    {
        if ( number_range( 1, 4 ) != 2 )
        {
          act("You stare deeply into $N's eyes, but nothing happens.",ch,NULL,victim,TO_CHAR);
          act("$n stares deeply into your eyes.",ch,NULL,victim,TO_VICT);
          act("$n stares deeply into $N's eyes, but nothing happens.",ch,NULL,victim,TO_NOTVICT);
	    return;
        }

      act("You stare into $N's eyes, and $E flees in terror.",ch,NULL,victim,TO_CHAR);
      act("$n stares deeply into your eyes, and you are filled with terror.",ch,NULL,victim,TO_VICT);
      act("$n stares deeply into $N's eyes, and $E flees in terror.",ch,NULL,victim,TO_NOTVICT);
      do_flee( victim, "" );
      return;
    }

  return;

}

*/

