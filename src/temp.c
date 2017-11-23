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
#include <unistd.h>

void do_mastery( CHAR_DATA *ch, char *argument )
{
    OBJ_INDEX_DATA *pObjIndex;
    OBJ_DATA *obj;
    char arg[MAX_INPUT_LENGTH];
    int vnum = 0;
    int stancecount;
    int spellcount;
    int	weaponcount;
    
    weaponcount = 0;
    weaponcount += ch->wpn[0];
    weaponcount += ch->wpn[1];
    weaponcount += ch->wpn[2];
    weaponcount += ch->wpn[3];
    weaponcount += ch->wpn[4];
    weaponcount += ch->wpn[5];
    weaponcount += ch->wpn[6];
    weaponcount += ch->wpn[7];
    weaponcount += ch->wpn[8];
    weaponcount += ch->wpn[9];
    weaponcount += ch->wpn[10];
    weaponcount += ch->wpn[11];
    weaponcount += ch->wpn[12];

    stancecount = 0;
    stancecount += ch->stance[STANCE_BULL];
    stancecount += ch->stance[STANCE_VIPER];
    stancecount += ch->stance[STANCE_CRANE];
    stancecount += ch->stance[STANCE_MONGOOSE];
    stancecount += ch->stance[STANCE_CRAB];
    stancecount += ch->stance[STANCE_TIGER];
    stancecount += ch->stance[STANCE_MANTIS];
    stancecount += ch->stance[STANCE_DRAGON];
    stancecount += ch->stance[STANCE_SWALLOW];
    stancecount += ch->stance[STANCE_MONKEY];
    stancecount += ch->stance[STANCE_RABBIT];
    stancecount += ch->stance[STANCE_COUGAR];
    stancecount += ch->stance[STANCE_EAGLE];
    stancecount += ch->stance[STANCE_BEAR];
    stancecount += ch->stance[STANCE_COBRA];

    spellcount = 0;
    spellcount += ch->spl[0];
    spellcount += ch->spl[1];
    spellcount += ch->spl[2];
    spellcount += ch->spl[3];
    spellcount += ch->spl[4];

   if((spellcount < 1000) || (stancecount < 3000) || (weaponcount < 2600))
   {
	send_to_char("You are not worthy of creating mastery items.\n\r",ch);
	return;
   }

   if(ch->max_hit < 20000)
   {
	send_to_char("You don't have the 20000 Hitpoints needed to create it.\n\r",ch);
	return;
   }

    one_argument( argument,arg );
    if(IS_NPC(ch)) return;
    
    if( arg[0] == '\0')
    {
	send_to_char("What item do you wish to create?\n\r",ch);
	send_to_char("Choose from Speed, Vision and Protecion.\n\r",ch);
	send_to_char("Healing, Peace and the Beast.\n\r",ch);
	return;
    }
    
    if( !str_cmp(arg, "peace") )
      {
        send_to_char("Sorry But Peace Mastery is Offline.\n\r",ch);
        return;
      }

    if( !str_cmp(arg, "speed") ) vnum = 29522;
    else if( !str_cmp(arg, "vision") ) vnum = 29523;
    else if( !str_cmp(arg, "protection") ) vnum = 29524;
    else if( !str_cmp(arg, "healing") ) vnum = 29525;
  /*  else if( !str_cmp(arg, "peace") ) vnum = 29526; */
//    else if( !str_cmp(arg, "beast") ) vnum = 29527;
    else
    {
	send_to_char("Either speed, vision, protecion, healing,\n\r",ch);
	send_to_char("peace.\n\r",ch);
	return;
    }
    if( ch->practice < 100 || ch->pcdata->quest < 500)
    {
	send_to_char("It costs 100 primal & 500 quest points to create a Mastery Item.\n\r",ch);
	return;
    }

    if( vnum == 0 || (pObjIndex = get_obj_index( vnum )) == NULL)
    {
	send_to_char("Missing Object, barf wibble.\n\r",ch);
	return;
    }

    obj = create_object(pObjIndex, 50);
    if(IS_SET(obj->quest, QUEST_ARTIFACT )) REMOVE_BIT( obj->quest, QUEST_ARTIFACT );    
    SET_BIT( obj->quest, QUEST_MASTERY );
    free_string(obj->questowner);            
    obj->questowner = str_dup(ch->name);    
    free_string(obj->questmaker);              
    obj->questmaker = str_dup(ch->name);   
    obj->points = 500;
    obj_to_char(obj,ch);
    act("$p appears in your hands.",ch,obj,NULL,TO_CHAR);
    act("$p appears in $n's hands.",ch,obj,NULL,TO_ROOM);
    ch->practice -= 100;
    ch->pcdata->quest -= 500;
    ch->max_hit -= 1000;
    ch->hit = ch->max_hit;
    return;
    
}
