/***************************************************************************
 *  Original Diku Mud copyright (C) 1990, 1991 by Sebastian Hammer,        *
 *  Michael Seifert, Hans Henrik St{rfeldt, Tom Madsen, and Katja Nyboe.   *
 *                                                                         *
 *  Merc Diku Mud improvments copyright (C) 1992, 1993 by Michael          *
 *  Chastain, Michael Quan, and Mitchell Tse.                              *
 *                                                                         *
 *  In order to use any part of this Merc Diku Mud, you must comply with   *
 *  both the original Diku license in 'license.doc' as well the Merc       
*
 *  license in 'license.txt'.  In particular, you may not remove either of *
 *  these copyright notices.                                               *
 *                                                                         *
 *  Much time and thought has gone into this software and you are          *
 *  benefitting.  We hope that you share your changes too.  What goes      *
 *  around, comes around.                                                  *
 ***************************************************************************/
 /***************************************************************************
 *  God Wars Mud originally written by KaVir aka Richard Woolcock.         *
 *  Changes done to the code done by Sage aka Walter Howard, this mud is   *
 *  for the public, however if you run this code it means you agree        *
 *  to the license.low, license.gw, and license.merc have fun. :)          *
***************************************************************************/

#if defined(macintosh)
#include <types.h>
#else
#include <sys/types.h>
#endif
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "merc.h"
#include "dragon.h"

char *	const	where_name	[] =
{
    "{w.{D.{b:{B:{wLight         {B:{b:{D.{w.{x ",
    "{w.{D.{b:{B:{wOn Finger     {B:{b:{D.{w.{x ",
    "{w.{D.{b:{B:{wOn Finger     {B:{b:{D.{w.{x ",
    "{w.{D.{b:{B:{wAround Neck   {B:{b:{D.{w.{x ",
    "{w.{D.{b:{B:{wAround Neck   {B:{b:{D.{w.{x ",
    "{w.{D.{b:{B:{wOn Body       {B:{b:{D.{w.{x ",
    "{w.{D.{b:{B:{wOn Head       {B:{b:{D.{w.{x ",
    "{w.{D.{b:{B:{wOn Legs       {B:{b:{D.{w.{x ",
    "{w.{D.{b:{B:{wOn Feet       {B:{b:{D.{w.{x ",
    "{w.{D.{b:{B:{wOn Hands      {B:{b:{D.{w.{x ",
    "{w.{D.{b:{B:{wOn Arms       {B:{b:{D.{w.{x ",
    "{w.{D.{b:{B:{wShield        {B:{b:{D.{w.{x ",
    "{w.{D.{b:{B:{wAround Body   {B:{b:{D.{w.{x ",
    "{w.{D.{b:{B:{wAround Waist  {B:{b:{D.{w.{x ",
    "{w.{D.{b:{B:{wAround Wrist  {B:{b:{D.{w.{x ",
    "{w.{D.{b:{B:{wAround Wrist  {B:{b:{D.{w.{x ",
    "{w.{D.{b:{B:{wRight Hand    {B:{b:{D.{w.{x ",
    "{w.{D.{b:{B:{wLeft Hand     {B:{b:{D.{w.{x ",
    "{w.{D.{b:{B:{wThird Hand    {B:{b:{D.{w.{x ",
    "{w.{D.{b:{B:{wFourth Hand   {B:{b:{D.{w.{x ",
    "{w.{D.{b:{B:{wOn Face       {B:{b:{D.{w.{x ",
    "{w.{D.{b:{B:{wLeft Scabbard {B:{b:{D.{w.{x ",
    "{w.{D.{b:{B:{wRight Scabbard{B:{b:{D.{w.{x ",
    "{w.{D.{b:{B:{wSpecial       {B:{b:{D.{w.{x ",
    "{w.{D.{b:{B:{wFloating      {B:{b:{D.{w.{x ",
    "{w.{D.{b:{B:{wMedal         {B:{b:{D.{w.{x ",
    "{w.{D.{b:{B:{wBodyart       {B:{b:{D.{w.{x ",
};



/*
 * Local functions.
 * Changed by Kyrlin to add altreal colour 20/01/00
 */
char *	format_obj_to_char	args( ( OBJ_DATA *obj, CHAR_DATA *ch,
				    bool fShort ) );
void	show_char_to_char_0	args( ( CHAR_DATA *victim, CHAR_DATA *ch ) );
void	show_char_to_char_1	args( ( CHAR_DATA *victim, CHAR_DATA *ch ) );
void	show_char_to_char	args( ( CHAR_DATA *list, CHAR_DATA *ch ) );
bool	check_blind		args( ( CHAR_DATA *ch ) );

void	evil_eye		args( ( CHAR_DATA *victim, CHAR_DATA *ch ) );
void 	check_left_arm		args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );
void 	check_right_arm		args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );
void 	check_left_leg		args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );
void 	check_right_leg		args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );

void	obj_score		args( ( CHAR_DATA *ch, OBJ_DATA *obj ) );
/* Changed by Tracker 03/13/03 to get rid of annoying error message */
int number_attacks args(( CHAR_DATA *ch ));
bool fair_fight args((CHAR_DATA *ch, CHAR_DATA *victim));

char *format_obj_to_char( OBJ_DATA *obj, CHAR_DATA *ch, bool fShort )
{
    static char buf[MAX_STRING_LENGTH];

    buf[0] = '\0';
    if ( IS_SET(obj->quest, QUEST_ARTIFACT)) strcat(buf, "{D({wArtifact{D){x " );
    else if ( IS_SET(obj->quest, QUEST_MASTERY)) strcat(buf, "{D({cMastery{D){x " );
    else if ( IS_SET(obj->quest, QUEST_PRIZE)) strcat(buf, "{D({CPrize#D)#n ");
    else if ( IS_SET(obj->quest, QUEST_RELIC)) strcat(buf, "{D({RRelic{D)#n " );
    else if ( IS_SET(obj->quest, QUEST_IMBUE)) strcat(buf, "{D({bI{Bm{wbu{Be{bd{D){x ");
    else if ( obj->points < 750 && obj->points != 0) strcat(buf, "{D({wA{Wn{Dcie{Wn{wt{D){x ");
    else if ( obj->points < 1250 && obj->points != 0 ) strcat(buf, "{D({cLe{Cge{Dn{Cda{cry{D)#n ");
    else if ( obj->points < 3000 && obj->points !=0) strcat(buf, "{D({bMy{Bt{Whi{Bc{bal{D)#n " );
    else if ( obj->points > 3000 && obj->points !=0) strcat(buf, "{D({yPr{Yic{We{Yle{yss{x{D)#n " );
    else if (IS_WEAP(obj, WEAPON_FLAMING)) strcat(buf,"{D({YF{yl{ra{Rm{ri{yn{Yg{D)#n");
    if ( IS_SET(obj->quest,ITEM_EQUEST)) strcat(buf,"{D({yPr{Yic{We{Yle{yss{D)#n " );
    if ( IS_SET(obj->quest, QUEST_BLOODA)) strcat(buf, "{D(#rB#Rl#re#Re#rd#Ri#rn#Rg{D)#n " );
    if ( IS_OBJ_STAT(obj, ITEM_INVIS)     )   strcat( buf, "{D(Invis)#n " );
    if ( IS_AFFECTED(ch, AFF_DETECT_EVIL)
         && !IS_OBJ_STAT(obj, ITEM_ANTI_GOOD)
         && IS_OBJ_STAT(obj, ITEM_ANTI_EVIL)   )   strcat( buf, "{D(#bBlue Aura{D)#n "  );
    else if ( IS_AFFECTED(ch, AFF_DETECT_EVIL)
         && IS_OBJ_STAT(obj, ITEM_ANTI_GOOD)
         && !IS_OBJ_STAT(obj, ITEM_ANTI_EVIL)   )   strcat( buf, "{D(#rRed Aura{D)#N "  );
    else if ( IS_AFFECTED(ch, AFF_DETECT_EVIL)
         && IS_OBJ_STAT(obj, ITEM_ANTI_GOOD)
         && !IS_OBJ_STAT(obj, ITEM_ANTI_NEUTRAL)
         && IS_OBJ_STAT(obj, ITEM_ANTI_EVIL)   )   strcat( buf, "{D(#yYellow Aura{D)#n "  );
    if ( IS_AFFECTED(ch, AFF_DETECT_MAGIC)
         && IS_OBJ_STAT(obj, ITEM_MAGIC)  )   strcat( buf, "{D({BMa{bg{Wi{bc{Bal{D)#n "   );
    if ( IS_OBJ_STAT(obj, ITEM_GLOW)      )   strcat( buf, "{D({gG{Gl{go{Gw{D)#n "   );
    if ( IS_OBJ_STAT(obj, ITEM_HUM)       )   strcat( buf, "{D({yH{Yu{ym{D)#n "   );
    if ( IS_OBJ_STAT(obj, ITEM_SHADOWPLANE) &&
	 obj->in_room != NULL &&
	!IS_AFFECTED(ch,AFF_SHADOWPLANE) )    strcat( buf,"{D({mAstral plane{D)#n " );
    if (!IS_OBJ_STAT(obj, ITEM_SHADOWPLANE) &&
	 obj->in_room != NULL &&
	 IS_AFFECTED(ch,AFF_SHADOWPLANE) )    strcat( buf, "{D(#nNormal plane{D)#n " );

    if ( fShort )
    {
	if ( obj->short_descr != NULL )
	    strcat( buf, obj->short_descr );
	if ( obj->condition < 100) strcat(buf, " {R*damaged*{x");
    }
    else
    {
	if ( obj->description != NULL )
	    strcat( buf, obj->description );
    }

    return buf;
}



int char_hitroll( CHAR_DATA *ch )
{
    OBJ_DATA *obj;
    int hr = GET_HITROLL(ch);
    int min;
    int iWear;

    if (!IS_NPC(ch))
    {
	if (IS_CLASS(ch,CLASS_VAMPIRE) && ch->pcdata->stats[UNI_RAGE] > 0)
	    hr += ch->pcdata->stats[UNI_RAGE];
	if(IS_VAMPIRE(ch) && weather_info.sunlight == SUN_DARK)
	{
	 if ( ch->pcdata->rank == AGE_METHUSELAH ) hr += 150;
            else if ( ch->pcdata->rank == AGE_ELDER ) hr += 125;
            else if ( ch->pcdata->rank == AGE_ANCILLA ) hr += 100;
            else if ( ch->pcdata->rank == AGE_NEONATE ) hr += 75;
	    else hr += 50;
	}

        if (IS_CLASS(ch, CLASS_NINJA) && ch->pcdata->powerss[BLACK_BELT] > 0)
        { hr += ch->pcdata->powerss[BLACK_BELT] * 20; }
	if (IS_CLASS(ch,CLASS_NINJA) && ch->pcdata->stats[UNI_RAGE] > 0)
	    hr += ch->pcdata->stats[UNI_RAGE];

	if(IS_SHAKRIN(ch)) hr+= 1000;

        if (IS_POLYAFF(ch, POLY_NEWZULO) )
            hr += 300;
	if (IS_POLYAFF(ch, POLY_ZULO) )
	    hr += 200;
	if (IS_CLASS(ch, CLASS_NINJA) && ch->pcdata->powerss[NPOWER_CHIKYU] >= 6
	&& ch->pcdata->powerss[HARA_KIRI] > 0)
	hr += 100;

if (ch->hit < ch->max_hit && !IS_SET(ch->newbits, NEW_NOPAIN))
    {
// 	if (!IS_NPC(ch) && IS_WEREWOLF(ch) && get_auspice(ch, AUSPICE_PHILODOX) > 0)
//	return hr;
 	min = hr * 0.75;
 	if (ch->hit < 1 || ch->max_hit < 1)
 	    hr *= 0.75;
 	else
 	{
	    hr = (hr * ch->hit) / ch->max_hit;
	    if (hr < min) hr = min;
 	}
    } 

	if (IS_SET(ch->special,SPC_WOLFMAN) && ch->pcdata->stats[UNI_RAGE] > 0)
	    hr += ch->pcdata->stats[UNI_RAGE];
/*
	if (IS_CLASS( ch, CLASS_DEMON) && ch->pcdata->stats[DEMON_POWER] > 0)
	    hr += ((ch->pcdata->stats[DEMON_POWER]) * ch->pcdata->stats[DEMON_POWER]);
	if (IS_SET(ch->special,SPC_CHAMPION) && ch->pcdata->stats[DEMON_POWER] > 0)
	    hr += ((ch->pcdata->stats[DEMON_POWER]) * ch->pcdata->stats[DEMON_POWER]);
*/
	if (IS_DEMON(ch))
	{
//	if (IS_SET(ch->warp, WARP_HITROLL)) hr += 100;
	 for( iWear = 0; iWear < MAX_WEAR; iWear++ )
	 {
	    if( (obj = get_eq_char( ch,  iWear ) ) != NULL )
	    {
		if(IS_SET(obj->spectype, SITEM_DEMONIC))
		{
			if(obj->specpower == ARM_BLACK) hr += 5;
			if(obj->specpower == ARM_RED) hr += 10;
			if(obj->specpower == ARM_COPPER) hr += 15;
			if(obj->specpower == ARM_BRONZE) hr += 20;
			if(obj->specpower == ARM_BRASS) hr += 25;
		}
	    }
	 }
	}

	if (IS_CLASS(ch, CLASS_HIGHLANDER) && (get_eq_char(ch, WEAR_WIELD) != NULL))
	{
	int wpn = ch->wpn[1];
	if (wpn >= 500)
		hr += 500;
	else
		hr += wpn;
	}
    }
return hr;
}

int char_damroll( CHAR_DATA *ch )
{
    OBJ_DATA *obj;
    int dr = GET_DAMROLL(ch);
    int min;    
    int iWear;

if (!IS_NPC(ch))
    {
	if (IS_CLASS(ch,CLASS_VAMPIRE) && ch->pcdata->stats[UNI_RAGE] > 0)
	    dr += ch->pcdata->stats[UNI_RAGE];
        if(IS_VAMPIRE(ch) && weather_info.sunlight == SUN_DARK) 
        {
         if ( ch->pcdata->rank == AGE_METHUSELAH ) dr += 150;
            else if ( ch->pcdata->rank == AGE_ELDER ) dr += 125;
            else if ( ch->pcdata->rank == AGE_ANCILLA ) dr += 100;
            else if ( ch->pcdata->rank == AGE_NEONATE ) dr += 75; 
            else dr += 50;
        } 

	if (IS_POLYAFF(ch, POLY_ZULO))
	{
	    dr +=200;
	}
	
	if (IS_SHAKRIN(ch)) dr += 1000;

        if (IS_CLASS(ch, CLASS_NINJA) && ch->pcdata->powerss[BLACK_BELT] > 0)
        { dr += ch->pcdata->powerss[BLACK_BELT] * 20; }
	if (IS_CLASS(ch, CLASS_NINJA) && ch->pcdata->stats[UNI_RAGE] > 0)
	    dr += ch->pcdata->stats[UNI_RAGE];
	if (IS_CLASS(ch, CLASS_PALADIN)) 
	{
		dr += 25;
	}
	else if (IS_SET(ch->special,SPC_WOLFMAN) && ch->pcdata->stats[UNI_RAGE] > 0)
	    dr += ch->pcdata->stats[UNI_RAGE];
/*
	else if (IS_CLASS( ch,CLASS_DEMON ) && ch->pcdata->stats[DEMON_POWER] > 0)
	    dr += ((ch->pcdata->stats[DEMON_POWER]) * ch->pcdata->stats[DEMON_POWER]);
	if (IS_SET(ch->special,SPC_CHAMPION) && ch->pcdata->stats[DEMON_POWER] > 0)
	    dr += ((ch->pcdata->stats[DEMON_POWER]) * ch->pcdata->stats[DEMON_POWER]);
*/
    if (IS_DEMON(ch))
        {
//	if (IS_SET(ch->warp, WARP_DAMROLL)) dr += 100;

         for( iWear = 0; iWear < MAX_WEAR; iWear++ )
         {
            if( (obj = get_eq_char( ch,  iWear ) ) != NULL )
            {
                if(IS_SET(obj->spectype, SITEM_DEMONIC))
               {
                        if(obj->specpower == ARM_BLACK) dr += 5;
                        if(obj->specpower == ARM_RED) dr += 10;
                        if(obj->specpower == ARM_COPPER) dr += 15;
                        if(obj->specpower == ARM_BRONZE) dr += 20;
                        if(obj->specpower == ARM_BRASS) dr += 25;
                }
            }
         }
        }


  if (ch->hit < ch->max_hit && !IS_SET(ch->newbits, NEW_NOPAIN))
    {
//    if (!IS_NPC(ch) && IS_WEREWOLF(ch) && get_auspice(ch, AUSPICE_PHILODOX) > 0)
// 	    return dr;
 	min = dr * 0.75;
 	if (ch->hit < 1 || ch->max_hit < 1)
 	    dr *= 0.75;
 	else
	{
 	    dr = (dr * ch->hit) / ch->max_hit;
 	    if (dr < min) dr = min;
 	}
    }
 
	if (IS_NINJA(ch) && ch->pcdata->powerss[NPOWER_CHIKYU] >= 6 && ch->pcdata->powerss[HARA_KIRI] > 0)
	dr += 50;

	}

	if (IS_CLASS(ch, CLASS_HIGHLANDER) && (get_eq_char(ch, WEAR_WIELD) != NULL))

//	if (IS_HIGHLANDER(ch) && (get_eq_char(ch, WEAR_WIELD) != NULL))
	{
	int wpn = ch->wpn[1];
	if (wpn >= 500)
		dr += 500;
	else
		dr += wpn;
	}
//    }
        else if (IS_POLYAFF(ch, POLY_NEWZULO))
            dr += 300;
    return dr;

}

int char_ac( CHAR_DATA *ch )
{
    int a_c = GET_AC(ch);



    if (!IS_NPC(ch))
    {

	if (IS_CLASS(ch, CLASS_HIGHLANDER) && (get_eq_char(ch, WEAR_WIELD) != NULL))
/*IS_SET(obj->spectype,SITEM_HIGHLANDER)) */
/* mog IS_ITEMAFF(ch, ITEMA_HIGHLANDER))*/
	{

/* hit/dam bug... Nightstar removed. 
  if (ch->hit < ch->max_hit && !IS_SET(ch, NEW_NOPAIN))
    {
 //	if (!IS_NPC(ch) && IS_WEREWOLF(ch) && get_auspice(ch,AUSPICE_PHILODOX) > 0)
 //	    return a_c;
 	if (ch->hit < 1 || ch->max_hit < 1)
 	    a_c += 250;
 	else
 	    a_c += (ch->hit / ch->max_hit * 250);
    }
*/
 	    int wpn = ch->pcdata->powers[HPOWER_WPNSKILL];
 	    if (wpn == 1 || wpn == 3)
 		a_c -= ch->wpn[wpn];
 	wpn = ch->wpn[1];
 	a_c -= wpn;
 	}

   
if (IS_WEREWOLF(ch) && ch->gifts[CHILDRENOFGAIA] < 1)
{
a_c += -100;
}
	else if (IS_CLASS(ch, CLASS_PALADIN)) 
	{
		a_c += -100;
	}
    }
    return a_c;
}

/*
 * Show a list to a character.
 * Can coalesce duplicated items.
 */
void show_list_to_char( OBJ_DATA *list, CHAR_DATA *ch, bool fShort, bool fShowNothing )
{
    char buf[MAX_STRING_LENGTH];
    char **prgpstrShow;
    int *prgnShow;
    char *pstrShow;
    OBJ_DATA *obj;
    int nShow;
    int iShow;
    int count;
    bool fCombine;

    if ( ch->desc == NULL )
	return;

    /*
     * Alloc space for output lines.
     */
    count = 0;
    for ( obj = list; obj != NULL; obj = obj->next_content )
	count++;
    prgpstrShow	= alloc_mem( count * sizeof(char *) );
    prgnShow    = alloc_mem( count * sizeof(int)    );
    nShow	= 0;

    /*
     * Format the list of objects.
     */
    for ( obj = list; obj != NULL; obj = obj->next_content )
    { 
	if (!IS_NPC(ch) && ch->pcdata->chobj != NULL && obj->chobj != NULL && obj->chobj == ch)
	    continue;
	if ( obj->wear_loc == WEAR_NONE && can_see_obj( ch, obj ) )
	{
	    pstrShow = format_obj_to_char( obj, ch, fShort );
	    fCombine = FALSE;

	    if ( IS_NPC(ch) || IS_SET(ch->act, PLR_COMBINE) )
	    {
		/*
		 * Look for duplicates, case sensitive.
		 * Matches tend to be near end so run loop backwords.
		 */
		for ( iShow = nShow - 1; iShow >= 0; iShow-- )
		{
		    if ( !strcmp( prgpstrShow[iShow], pstrShow ) )
		    {
			prgnShow[iShow]++;
			fCombine = TRUE;
			break;
		    }
		}
	    }

	    /*
	     * Couldn't combine, or didn't want to.
	     */
	    if ( !fCombine )
	    {
		prgpstrShow [nShow] = str_dup( pstrShow );
		prgnShow    [nShow] = 1;
		nShow++;
	    }
	}
    }

    /*
     * Output the formatted list.
     */
    for ( iShow = 0; iShow < nShow; iShow++ )
    {
	if ( IS_NPC(ch) || IS_SET(ch->act, PLR_COMBINE) )
	{
	    if ( prgnShow[iShow] != 1 )
	    {
		sprintf( buf, "(%2d) ", prgnShow[iShow] );
		send_to_char( buf, ch );
	    }
	    else
	    {
		send_to_char( "     ", ch );
	    }
	}
	send_to_char( prgpstrShow[iShow], ch );
	send_to_char( "\n\r", ch );
	free_string( prgpstrShow[iShow] );
    }

    if ( fShowNothing && nShow == 0 )
    {
	if ( IS_NPC(ch) || IS_SET(ch->act, PLR_COMBINE) )
	    send_to_char( "     ", ch );
	send_to_char( "Nothing.\n\r", ch );
    }

    /*
     * Clean up.
     */
    free_mem( prgpstrShow, count * sizeof(char *) );
    free_mem( prgnShow,    count * sizeof(int)    );

    return;
}



void show_char_to_char_0( CHAR_DATA *victim, CHAR_DATA *ch )
{
    char buf[MAX_STRING_LENGTH];
    char buf2[MAX_STRING_LENGTH];
    char buf3[MAX_STRING_LENGTH];
    char buf4[MAX_STRING_LENGTH];
    char mount2[MAX_STRING_LENGTH];
    CHAR_DATA *mount;

    buf[0] = '\0';
    buf2[0] = '\0';
    buf3[0] = '\0';

    if (!IS_NPC(victim) && victim->pcdata->chobj != NULL )
	return;

    if ((mount = victim->mount) != NULL && IS_SET(victim->mounted, IS_MOUNT))
	return;



    if ( IS_HEAD(victim, LOST_HEAD) && IS_AFFECTED(victim, AFF_POLYMORPH))
	strcat( buf, "     " );
    else {
                if ( !IS_NPC(victim) && (! IS_IMMORTAL(victim) ) )
                {
                    send_to_char("#B[#n", ch);
                    if ( IS_ITEMAFF(victim, ITEMA_SHOCKSHIELD) )
                        send_to_char("#yS#n", ch);
                    else
                        send_to_char("#y.#n", ch);
                    if ( IS_ITEMAFF(victim, ITEMA_FIRESHIELD) )
                        send_to_char("#rF#n", ch);
                    else
                        send_to_char("#r.#n", ch);
                    if ( IS_ITEMAFF(victim, ITEMA_ICESHIELD) )
                        send_to_char("#cI#n", ch);
                    else
                        send_to_char("#c.#n", ch);
                    if ( IS_ITEMAFF(victim, ITEMA_ACIDSHIELD) )
                        send_to_char("#gA#n", ch);
                    else
                        send_to_char("#g.#n", ch);
                    if ( IS_ITEMAFF(victim, ITEMA_CHAOSSHIELD) )
                        send_to_char("#PC#n", ch);
                    else
                        send_to_char("#P.#n", ch);
                    if ( IS_ITEMAFF(victim, ITEMA_REFLECT) )
                        send_to_char("#eD#n", ch);
                    else
                        send_to_char("#e.#n", ch);
                    send_to_char("#B]#n ", ch);
                }
	if (!IS_NPC(victim) && victim->desc==NULL ) strcat( buf,"#B(#nLink-Dead#B)#n ");
    if(!IS_VAMPIRE(victim) && !IS_SET(victim->flag2, VAMP_OBJMASK) )
     {
    	if ( IS_AFFECTED(victim, AFF_INVISIBLE)   ) strcat( buf, "{D(Invis)#n "   );
    	if ( IS_AFFECTED(victim, AFF_HIDE)        ) strcat( buf, "{D({wHide{D)#n "    );
    	if ( IS_AFFECTED(victim, AFF_CHARM)       ) strcat( buf, "{D(#pCharmed{D)#n " );
    	if ( IS_AFFECTED(victim, AFF_PASS_DOOR)  ||
         IS_AFFECTED(victim, AFF_ETHEREAL)    )
	 strcat( buf, "{D({gTranslucent{D)#n ");
    	if ( IS_AFFECTED(victim, AFF_FAERIE_FIRE) ) strcat( buf, "{D(#pPink Aura{D)#n "  );
    	if ( IS_EVIL(victim)
    	&&   IS_AFFECTED(ch, AFF_DETECT_EVIL)     ) strcat( buf,"{D(#rRed Aura{D)#n "   );
    	if ( IS_AFFECTED(victim, AFF_SANCTUARY)   ) strcat( buf, "{D(#wSanct{D)#n " );
	if ( IS_AFFECTED(victim, AFF_STEELSHIELD) ) strcat( buf, "{D({BMajesty{D)#n ");
     }
    }
    if ( IS_AFFECTED(ch, AFF_SHADOWPLANE) && !IS_AFFECTED(victim, AFF_SHADOWPLANE))
	strcat( buf, "{D(#nNormal plane{D)#n "     );
    else if ( !IS_AFFECTED(ch, AFF_SHADOWPLANE) && IS_AFFECTED(victim, AFF_SHADOWPLANE))
	strcat( buf, "{D({mAstral plane{D)#n "     );

		if ((IS_SET(victim->act, PLR_INCOG)) && (IS_IMMORTAL(victim)))
			strcat( buf, "{D(Incog)#n ");
		if (IS_SET(victim->act, PLR_WIZINVIS) && IS_IMMORTAL(victim))
			strcat( buf, "{D(Winvis)#n ");

if ( !IS_NPC(ch) && !IS_NPC(victim) && IS_HERO(ch) && IS_HERO(victim) && victim->level < 7 )
    {
       if (IS_VAMPIRE(victim) && !IS_SET(victim->flag2, VAMP_OBJMASK) )
           strcat(buf, "{D({rKindred{D)#n ");
       if (IS_WEREWOLF(victim) )
           strcat(buf, "{D({GGarou{D)#n ");
       if (IS_MONK(victim) )
           strcat(buf, "{D({cMonk{D)#n "); 
       if (IS_DEMON(victim) )
           strcat(buf, "{D({RFallen{D)#n ");
       if (IS_NINJA(victim) )
           strcat(buf, "{D({mNinja{D)#n "); 
       if (IS_MAGE(victim) )
           strcat(buf, "{D(#.Warlock{D)#n ");
       if (IS_HIGHLANDER(victim) )
           strcat(buf, "{D({CHighlander{D)#n ");
       if (IS_CLASS(victim, CLASS_DRAGON) )
           strcat(buf, "{D(#.Dragon{D)#n ");
       if (IS_SHAKRIN(victim) )
           strcat(buf, "{D(Bugged - Shakrin){x");
       if(IS_WRAITH(victim) )
           strcat(buf, "{D(Bugged - Wraith)#n ");
       if (IS_CLASS(victim, CLASS_DROW))
                strcat(buf, "{D({MDrow{D)#n ");
/*
       if (IS_CLASS(victim, CLASS_DRAGON))
                strcat(buf, "(Dragon) ");
       if (IS_CLASS(victim, CLASS_MOOGLE))
                strcat(buf, "(Moogle) "); */
}
       if ( !IS_NPC(ch) && IS_VAMPAFF(ch,VAM_AUSPEX) &&
         !IS_NPC(victim) && IS_CLASS(victim, CLASS_VAMPIRE) &&
          IS_VAMPAFF(victim, VAM_DISGUISED) )
    {
        strcat( buf, "(");
        strcat( buf, victim->name);
        strcat( buf, ") ");
    }
    if ( IS_AFFECTED(victim, AFF_FLAMING) ) 
    {
	if (IS_NPC(victim))
	    sprintf( buf2, "\n\r#r...%s is engulfed in blazing flames!#n", victim->short_descr );
	else if (!IS_NPC(victim) && IS_AFFECTED(victim,AFF_POLYMORPH))
	    sprintf( buf2, "\n\r#r...%s is engulfed in blazing flames!#n", victim->morph );
	else
	    sprintf( buf2, "\n\r#r...%s is engulfed in blazing flames!#n", victim->name );
    }

    if ( !IS_NPC(victim) && IS_HEAD(victim, LOST_HEAD) && IS_AFFECTED(victim, AFF_POLYMORPH)) 
    {
	if (IS_EXTRA(victim,GAGGED) && IS_EXTRA(victim,BLINDFOLDED))
	    sprintf( buf3, "...%s is gagged and blindfolded!", victim->morph );
	else if (IS_EXTRA(victim,GAGGED))
	    sprintf( buf3, "...%s is gagged!", victim->morph );
	else if (IS_EXTRA(victim,BLINDFOLDED))
	    sprintf( buf3, "...%s is blindfolded!", victim->morph );
    }

    if ( IS_HEAD(victim, LOST_HEAD) && IS_AFFECTED(victim, AFF_POLYMORPH))
    {
	strcat( buf, victim->morph );
	strcat( buf, " is lying here." );
    	strcat( buf, buf2 );
    	strcat( buf, buf3 );
    	strcat( buf, "\n\r" );
    	buf[5] = UPPER(buf[5]);
    	send_to_char( buf, ch );
	return;
    }

    if ( IS_EXTRA(victim, TIED_UP) ) 
    {
	if (IS_NPC(victim))
	    sprintf( buf3, "\n\r...%s is tied up", victim->short_descr );
	else if (!IS_NPC(victim) && IS_AFFECTED(victim,AFF_POLYMORPH))
	    sprintf( buf3, "\n\r...%s is tied up", victim->morph );
	else
	    sprintf( buf3, "\n\r...%s is tied up", victim->name );
	if (IS_EXTRA(victim,GAGGED) && IS_EXTRA(victim,BLINDFOLDED))
	    strcat( buf3, ", gagged and blindfolded!" );
	else if (IS_EXTRA(victim,GAGGED))
	    strcat( buf3, " and gagged!" );
	else if (IS_EXTRA(victim,BLINDFOLDED))
	    strcat( buf3, " and blindfolded!" );
	else
	    strcat( buf3, "!" );
    }

    if ( IS_AFFECTED(victim, AFF_WEBBED) ) 
    {
	if (IS_NPC(victim))
	    sprintf( buf4, "\n\r...%s is coated in a sticky web.", victim->short_descr );
	else if (!IS_NPC(victim) && IS_AFFECTED(victim,AFF_POLYMORPH))
	    sprintf( buf4, "\n\r...%s is coated in a sticky web.", victim->morph );
	else
	    sprintf( buf4, "\n\r...%s is coated in a sticky web.", victim->name );
	strcat( buf3, buf4 );
    }

    if ( IS_SET(victim->mflags, MAGE_EMBRACED2)
	|| IS_SET(victim->mflags, MAGE_EMBRACED1) )
    {
        if ( IS_NPC(victim))
  	  sprintf( buf4, "\n\r...%s is surrounded by earth.",victim->short_descr);
	else if ( !IS_NPC(victim) && IS_AFFECTED(victim,AFF_POLYMORPH))
	  sprintf( buf4, "\n\r...%s is surrounded by earth.",victim->morph);
	else
	  sprintf( buf4, "\n\r...%s is surrounded by earth.",victim->name);
    }


    if ( !IS_NPC(victim) && IS_AFFECTED(victim, AFF_POLYMORPH) )
	strcat( buf, victim->morph );
    else if ( victim->position == POS_STANDING && victim->long_descr[0] != '\0' && (mount = victim->mount) == NULL )
    {
	strcat( buf, victim->long_descr );
	send_to_char( buf, ch );
	return;
    }
    else
    	strcat( buf, PERS( victim, ch ) );

    if ((mount = victim->mount) != NULL && victim->mounted == IS_RIDING)
    {
	if (IS_NPC(mount))
	    sprintf( mount2, " is here riding %s", mount->short_descr );
	else
	    sprintf( mount2, " is here riding %s", mount->name );
	strcat( buf, mount2 );
	if (victim->position == POS_FIGHTING)
	{
	    strcat( buf, ", fighting " );
	    if ( victim->fighting == NULL )
	        strcat( buf, "thin air??" );
	    else if ( victim->fighting == ch )
	    strcat( buf, "YOU!" );
	    else if ( victim->in_room == victim->fighting->in_room )
	    {
	        strcat( buf, PERS( victim->fighting, ch ) );
	        strcat( buf, "." );
	    }
	    else
	        strcat( buf, "somone who left??" );
	}
	else strcat( buf, "." );
    }
    else if ( victim->position == POS_STANDING && IS_AFFECTED(victim, AFF_FLYING) )
	strcat( buf, " is hovering here" );
    else if ( victim->position == POS_STANDING && (!IS_NPC(victim) && (IS_VAMPAFF(victim, VAM_FLYING))) )
	strcat( buf, " is hovering here" );
/*
    else if (victim->position == POS_STANDING && (!IS_NPC(victim) &&
IS_CLASS(victim, CLASS_DROW) && IS_SET(victim->pcdata->powersd[1], DPOWER_LEVITATION)))
	strcat(buf, " is hovering here" );
*/
    else
    {
    	switch ( victim->position )
    	{
    	case POS_DEAD:     strcat( buf, " is DEAD!!" );              break;
    	case POS_MORTAL:   
	    if (IS_VAMPIRE(victim))
		strcat( buf, " is lying here torpored." );
	    else
		strcat( buf, " is lying here mortally wounded." );
	    break;
    	case POS_INCAP:    strcat( buf, " is lying here incapacitated." ); break;
    	case POS_STUNNED:  strcat( buf, " is lying here stunned." ); break;
    	case POS_SLEEPING: strcat( buf, " is sleeping here." );      break;
    	case POS_RESTING:  strcat( buf, " is resting here." );       break;
    	case POS_MEDITATING: strcat( buf, " is meditating here." );  break;
    	case POS_SITTING:  strcat( buf, " is sitting here." );       break;
    	case POS_STANDING:
	if(!IS_VAMPIRE(victim)  && IS_SET(victim->flag2, VAMP_OBJMASK) )
	{
	    if      (!IS_NPC(victim) && victim->stance[0] == STANCE_NORMAL)
		strcat( buf, " is here, crouched in a fighting stance." );
	    else if (!IS_NPC(victim) && victim->stance[0] == STANCE_VIPER)
		strcat( buf, " is here, crouched in a viper fighting stance." );
	    else if (!IS_NPC(victim) && victim->stance[0] == STANCE_CRANE)
		strcat( buf, " is here, crouched in a crane fighting stance." );
	    else if (!IS_NPC(victim) && victim->stance[0] == STANCE_CRAB)
		strcat( buf, " is here, crouched in a crab fighting stance." );
	    else if (!IS_NPC(victim) && victim->stance[0] == STANCE_MONGOOSE)
		strcat( buf, " is here, crouched in a mongoose fighting stance." );
	    else if (!IS_NPC(victim) && victim->stance[0] == STANCE_BULL)
		strcat( buf, " is here, crouched in a bull fighting stance." );
	    else if (!IS_NPC(victim) && victim->stance[0] == STANCE_MANTIS)
		strcat( buf, " is here, crouched in a mantis fighting stance." );
	    else if (!IS_NPC(victim) && victim->stance[0] == STANCE_DRAGON)
		strcat( buf, " is here, crouched in a dragon fighting stance." );
	    else if (!IS_NPC(victim) && victim->stance[0] == STANCE_TIGER)
		strcat( buf, " is here, crouched in a tiger fighting stance." );
	    else if (!IS_NPC(victim) && victim->stance[0] == STANCE_MONKEY)
		strcat( buf, " is here, crouched in a monkey fighting stance." );
	    else if (!IS_NPC(victim) && victim->stance[0] == STANCE_SWALLOW)
		strcat( buf, " is here, crouched in a swallow fighting stance." );
	    else if (!IS_NPC(victim) && victim->stance[0] == STANCE_RABBIT)
		strcat( buf, " is here, crouched in a rabbit fighting stance." );
	    else if (!IS_NPC(victim) && victim->stance[0] == STANCE_COBRA)
		strcat( buf, " is here, crouched in a cobra fighting stance." );
	    else if (!IS_NPC(victim) && victim->stance[0] == STANCE_BEAR)
		strcat( buf, " is here, crouched in a bear fighting stance." );
	    else if (!IS_NPC(victim) && victim->stance[0] == STANCE_COUGAR)
		strcat( buf, " is here, crouched in a cougar fighting stance." );
	    else if (!IS_NPC(victim) && victim->stance[0] == STANCE_EAGLE)
		strcat( buf, " is here, crouched in a eagle fighting stance." );

	    else
		strcat( buf, " is here." );
	}
	    break;
    	case POS_FIGHTING:
	    strcat( buf, " is here, fighting " );
	    if ( victim->fighting == NULL )
	        strcat( buf, "thin air??" );
	    else if ( victim->fighting == ch )
	    strcat( buf, "YOU!" );
	    else if ( victim->in_room == victim->fighting->in_room )
	    {
	        strcat( buf, PERS( victim->fighting, ch ) );
	        strcat( buf, "." );
	    }
	    else
	        strcat( buf, "somone who left??" );
	    break;
	}
    }

    strcat( buf, buf2 );
    strcat( buf, buf3 );
    strcat( buf, "\n\r" );
    buf[0] = UPPER(buf[0]);
    send_to_char( buf, ch );

    if ( !IS_NPC(ch) && IS_SET(ch->act, PLR_BRIEF) ) return;
    return;
}



void evil_eye( CHAR_DATA *victim, CHAR_DATA *ch )
{
    char buf [MAX_STRING_LENGTH];
    int tabletype;
 
    return;

    if (IS_NPC(victim)) return;
    if (victim == ch) return;
    if (ch->level != 3 || victim->level != 3) return;
    if (!IS_VAMPAFF(victim, VAM_DOMINATE)) return;
    if (victim->powertype != NULL && strlen(victim->powertype) > 1)
    {
	sprintf(buf,"\n\r%s\n\r",victim->powertype);
	send_to_char(buf,ch);
    }
    if (IS_SET(victim->spectype,EYE_SELFACTION) && victim->poweraction != NULL)
	interpret(victim,victim->poweraction);
    if (IS_SET(victim->spectype,EYE_ACTION) && victim->poweraction != NULL)
	interpret(ch,victim->poweraction);
    if (IS_SET(victim->spectype,EYE_SPELL) && victim->specpower > 0)
    {
	tabletype = skill_table[victim->specpower].target;
	(*skill_table[victim->specpower].spell_fun) (victim->specpower,victim->spl[tabletype],victim,ch);
    }
    return;
}


void show_char_to_char_1( CHAR_DATA *victim, CHAR_DATA *ch )
{
    char buf[MAX_STRING_LENGTH];
    OBJ_DATA *obj;
    int iWear;
    int percent;
    bool found;

    if ( can_see( victim, ch ) )
    {
	act( "$n looks at you.", ch, NULL, victim, TO_VICT    );
	act( "$n looks at $N.",  ch, NULL, victim, TO_NOTVICT );
    }

    if (!IS_NPC(ch) && IS_HEAD(victim,LOST_HEAD))
    {
	act( "$N is lying here.", ch, NULL, victim, TO_CHAR );
	return;
    }
    if ( victim->description[0] != '\0' )
    {
	send_to_char( victim->description, ch );
    }
    else
    {
	act( "You see nothing special about $M.", ch, NULL, victim, TO_CHAR );
    }

    if ( victim->max_hit > 0 )
	percent = ( 100 * victim->hit ) / victim->max_hit;
    else
	percent = -1;

    strcpy( buf, PERS(victim, ch) );

         if ( percent >= 100 ) strcat( buf, " is in perfect health.\n\r"  );
    else if ( percent >=  90 ) strcat( buf, " is slightly scratched.\n\r" );
    else if ( percent >=  80 ) strcat( buf, " has a few bruises.\n\r"     );
    else if ( percent >=  70 ) strcat( buf, " has some cuts.\n\r"         );
    else if ( percent >=  60 ) strcat( buf, " has several wounds.\n\r"    );
    else if ( percent >=  50 ) strcat( buf, " has many nasty wounds.\n\r" );
    else if ( percent >=  40 ) strcat( buf, " is bleeding freely.\n\r"    );
    else if ( percent >=  30 ) strcat( buf, " is covered in blood.\n\r"   );
    else if ( percent >=  20 ) strcat( buf, " is leaking guts.\n\r"       );
    else if ( percent >=  10 ) strcat( buf, " is almost dead.\n\r"        );
    else                       strcat( buf, " is DYING.\n\r"              );

    buf[0] = UPPER(buf[0]);
    send_to_char( buf, ch );
 /* Changed by Kyrlin to include admantiumhands (monk), flamehands (vamps), obeah (vamps),
  * night/shadowsight (various), new fly message (drow), new claw messages (various),
  * spiderarms (drow), typos (demon stuff), added colour, quills (ww) - 21 & 28 & 29 & 30/01/00
  */
if (!IS_NPC( victim ))
{
    if (IS_SET(victim->newbits, NEW_OBEAH))
    {
	if (victim->obeah_timer > 0)
	    act("$N has a brightly glowing eye in the centre of $S forehead.",ch,NULL,victim,TO_CHAR);
	else
	    act("$N has an eye in the centre of $S forehead.",ch,NULL,victim,TO_CHAR);
    }
    if (IS_VAMPAFF(victim, VAM_NIGHTSIGHT))
    {
        if (IS_AFFECTED(victim, AFF_SHADOWSIGHT))
        {
            if (IS_CLASS(victim, CLASS_WEREWOLF))
            act("$N's eyes are glowing a lambent #ggreen#n and are surrounded in #wshadow#n.",ch,NULL,victim,TO_CHAR);
        else 
            act("$N's eyes are glowing bright #rred#n and are surrounded in #wshadow#n.",ch,NULL,victim,TO_CHAR);
        }
    else if (IS_CLASS(victim, CLASS_WEREWOLF))
        act("$N's eyes are glowing a lambent #ggreen#n.",ch,NULL,victim,TO_CHAR);
    else 
        act("$N's eyes are glowing bright #rred#n.",ch,NULL,victim,TO_CHAR);
    }
    if (IS_VAMPAFF(victim, VAM_FANGS)) act("$N has a pair of long, pointed fangs.",ch,NULL,victim,TO_CHAR);
    if (IS_VAMPAFF(victim, VAM_CLAWS))
    {
        if (IS_CLASS(victim, CLASS_VAMPIRE))
        {
            if (IS_POLYAFF(victim, POLY_NEWZULO))
	      act("$N has long black talons extending from $S fingers.",ch,NULL,victim,TO_CHAR);
	      else if (IS_POLYAFF(victim, POLY_WOLF))
	      act("$N has razor sharp talons extending from $S paws.",ch,NULL,victim,TO_CHAR);
	      else if (IS_POLYAFF(victim, POLY_BAT))
	      act("$N has tiny, razor sharp claws extending from $S wing tips.",ch,NULL,victim,TO_CHAR);
	      else
	      act("$N has razor sharp claws protruding from under $S finger nails.",ch,NULL,victim,TO_CHAR);
        }
        if (IS_CLASS(victim, CLASS_WEREWOLF))
        {
            if(IS_GAR1(victim, WOLF_VISCERAL))
            {
                if(IS_GAR1(victim, WOLF_RAZORCLAWS) &&
                victim->gifts[SILVERFANGS] >= 2)
                act("$N has venom-dripping razor sharp silver talons extending from $S fingers.",ch,NULL,victim,TO_CHAR);
                else if(IS_GAR1(victim, WOLF_RAZORCLAWS) )
                act("$N has venom-dripping razor sharp talons extending from $S fingers.",ch,NULL,victim,TO_CHAR);
                else if(victim->gifts[SILVERFANGS] >= 2)
                act("$N has venom-dripping silver talons extending from $S fingers.",ch,NULL,victim,TO_CHAR);
                else
                act("$N has venom-dripping talons extending from $S fingers.",ch,NULL,victim,TO_CHAR);
            }
            else if(IS_GAR1(victim, WOLF_RAZORCLAWS) )
            {
                if(victim->gifts[SILVERFANGS] >=2)
                act("$N has razor sharp silver talons extending from $S fingers.",ch,NULL,victim,TO_CHAR);
                else
                act("$N has razor sharp talons extending from $S fingers.",ch,NULL,victim,TO_CHAR);
            }
            else if(victim->gifts[SILVERFANGS] >=2)
            act("$N has silver talons extending from $S fingers.",ch,NULL,victim,TO_CHAR);
            else
            act("$N has talons extending from $S fingers.",ch,NULL,victim,TO_CHAR);
        }
        if (IS_CLASS(victim, CLASS_NINJA))
        act("$N has iron claws attatched to $S wrists.",ch,NULL,victim,TO_CHAR);
        if (IS_CLASS(victim, CLASS_DEMON))
        act("$N has long black talons extending from $S fingers.",ch,NULL,victim,TO_CHAR);
    }
    if (IS_SET(victim->newbits, NEW_MONKFLAME))
    {
        if (IS_CLASS(victim, CLASS_VAMPIRE))
        {
            if (IS_POLYAFF(victim, POLY_WOLF))
            act("$N's paws are engulfed in blazing flames.",ch,NULL,victim,TO_CHAR);
            else if (IS_POLYAFF(victim, POLY_BAT))
            act("$N's wing tips are engulfed in blazing flames.",ch,NULL,victim,TO_CHAR);
            else
    	      act("$N's hands are engulfed in blazing flames.",ch,NULL,victim,TO_CHAR);
        }
    }
    if (IS_SET(victim->newbits, NEW_MONKADAM))
    act("$N's hands are as hard as adamantium.",ch,NULL,victim,TO_CHAR);
    if (IS_SET(victim->special, SPC_WOLFMAN))
    {
        if (IS_SET(victim->newbits, NEW_QUILLS))
        act("$N's body is covered in thick fur and quill-like spines.",ch,NULL,victim,TO_CHAR);
    else
        act("$N's body is covered in thick fur.",ch,NULL,victim,TO_CHAR);
    }
    if (IS_CLASS(victim, CLASS_DROW) && IS_POLYAFF(victim, POLY_SPIDERFORM) &&
        IS_SET(victim->pcdata->powersd[1], DPOWER_ARMS))
    act("$N has 8 spidery arms hanging from $S sides!", ch,NULL,victim,TO_CHAR);
    if (IS_CLASS(victim, CLASS_DROW) && IS_SET(victim->pcdata->powersd[1], DPOWER_LEVITATION))
    act("$N is levitating in the air.", ch,NULL,victim,TO_CHAR);
    else if (IS_AFFECTED(victim, AFF_FLYING)) act("$N is hovering in the air.",ch,NULL,victim,TO_CHAR);
    if (!IS_NPC(victim) && (IS_CLASS(victim, CLASS_DEMON) || IS_SET(victim->special, SPC_CHAMPION)))
    {
        if (IS_DEMAFF(victim, DEM_HORNS)) act("$N has a pair of pointed horns extending from $S head.",ch,NULL,victim,TO_CHAR);
        if (IS_DEMAFF(victim, DEM_HOOVES)) act("$N has a huge pair of hooves in place of $S feet.",ch,NULL,victim,TO_CHAR);
        if (IS_DEMAFF(victim, DEM_TAIL)) act("$N has a huge tail extended from $S back side.",ch,NULL,victim,TO_CHAR);
        if (IS_CLASS(ch, CLASS_DEMON) && IS_SET(ch->warp, WARP_KNIFE) ) act("$N has a knife stuck in $S head.",ch,NULL,victim,TO_CHAR); 
        if (IS_CLASS(ch, CLASS_DEMON) && IS_SET(ch->warp, WARP_SPOON) ) act("$N has a spoon stuck on $S ear.",ch,NULL,victim,TO_CHAR); 
        if (IS_CLASS(ch, CLASS_DEMON) && IS_SET(ch->warp, WARP_FORK) ) act("$N has a fork stuck in $S nose",ch,NULL,victim,TO_CHAR); 
        if (IS_DEMAFF(victim, DEM_WINGS))
        {
            if (IS_DEMAFF(victim, DEM_UNFOLDED))
            act("$N has a pair of batlike wings spread out from behind $S back.",ch,NULL,victim,TO_CHAR);
            else
            act("$N has a pair of batlike wings folded behind $S back.",ch,NULL,victim,TO_CHAR);
        }
    }
      if( IS_CLASS(victim, CLASS_DEMON) && IS_SET(victim->flag2, AFF2_IMP) )
      {
      act("$N has a little imp perched on $S shoulder.",ch,NULL,victim,TO_CHAR);
      }

	if( IS_SET(victim->flag2, AFF2_SPIRITGUARD) ) 
	{
          if( IS_CLASS(victim, CLASS_VAMPIRE) )
          {
              act("$N has a spirit floating just behind $S shoulder.",ch,NULL,victim,TO_CHAR);
          }
          else
          {
              act("$N has an unknown affect please inform Tracker!",ch,NULL,victim,TO_CHAR);
          }
	}
}
    found = FALSE;
    for ( iWear = 0; iWear < MAX_WEAR; iWear++ )
    {
	if ( ( obj = get_eq_char( victim, iWear ) ) != NULL
	&&   can_see_obj( ch, obj ) )
	{
	    if ( !found )
	    {
		send_to_char( "\n\r", ch );
		act( "$N is using:", ch, NULL, victim, TO_CHAR );
		found = TRUE;
	    }
	    send_to_char( where_name[iWear], ch );
	    if (IS_NPC(ch) || ch->pcdata->chobj == NULL || ch->pcdata->chobj != obj)
	    {
	    	send_to_char( format_obj_to_char( obj, ch, TRUE ), ch );
	    	send_to_char( "\n\r", ch );
	    }
	    else
		send_to_char( "you\n\r", ch);
	}
    }

    if ( victim != ch
    &&   !IS_NPC(ch)
    &&   !IS_HEAD(victim, LOST_HEAD)
    &&   number_percent( ) < ch->pcdata->learned[gsn_peek] )
    {
	send_to_char( "\n\rYou peek at the inventory:\n\r", ch );
	show_list_to_char( victim->carrying, ch, TRUE, TRUE );
    }

    return;
}



void show_char_to_char( CHAR_DATA *list, CHAR_DATA *ch )
{
    CHAR_DATA *rch;

    for ( rch = list; rch != NULL; rch = rch->next_in_room )
    {
	if ( rch == ch )
	    continue;

	if ( IS_IMMORTAL(ch) || IS_IMMORTAL(rch) )
	{
	    if ( !IS_NPC(rch)
	    &&   IS_SET(rch->act, PLR_WIZINVIS)
	    &&   get_trust( ch ) < rch->pcdata->wizinvis)
		continue;
	}
	else
	{
	    if ( !IS_NPC(rch)
	    && ( IS_SET(rch->act, PLR_WIZINVIS)
	    ||   IS_ITEMAFF(rch, ITEMA_VANISH) )
	    &&   !IS_SET(ch->act, PLR_HOLYLIGHT)
	    &&   !IS_ITEMAFF(ch, ITEMA_VISION) )
		continue;
	}

	if ( !IS_NPC(rch) && IS_HEAD(rch,LOST_HEAD))
	    continue;

	if ( !IS_NPC(rch) && IS_EXTRA(rch,EXTRA_OSWITCH))
	    continue;

	if ( can_see( ch, rch ) )
	    show_char_to_char_0( rch, ch );
	else if ( room_is_dark( ch->in_room )
	&&      ( IS_AFFECTED(rch, AFF_INFRARED)||(!IS_NPC(rch)&&IS_VAMPAFF(rch, VAM_NIGHTSIGHT))))
	{
	    send_to_char( "You see glowing red eyes watching YOU!\n\r", ch );
	}
    }

    return;
} 



bool check_blind( CHAR_DATA *ch )
{
    if ( !IS_NPC(ch) && IS_SET(ch->act, PLR_HOLYLIGHT) && !IS_AFFECTED(ch, AFF_TOTALBLIND))
	return TRUE;

    if ( IS_ITEMAFF(ch, ITEMA_VISION) && !IS_AFFECTED(ch, AFF_TOTALBLIND))
	return TRUE;

    if ( IS_HEAD(ch, LOST_EYE_L) && IS_HEAD(ch, LOST_EYE_R) )
    {
	send_to_char( "You have no eyes to see with!\n\r", ch );
	return FALSE;
    }

    if ( IS_EXTRA(ch, BLINDFOLDED) )
    {
	send_to_char( "You can't see a thing through the blindfold!\n\r", ch );
	return FALSE;
    }

    if ( IS_AFFECTED(ch, AFF_BLIND) && !IS_AFFECTED(ch, AFF_SHADOWSIGHT))
    {
	send_to_char( "You can't see a thing!\n\r", ch );
	return FALSE;
    }

    if ( IS_AFFECTED(ch, AFF_TOTALBLIND) && !IS_IMMORTAL(ch) )
    {
	send_to_char( "You can't see through this shroud of darkness!\n\r", ch );
	return FALSE;
    }
    return TRUE;
}



void do_look( CHAR_DATA *ch, char *argument )
{
    char buf  [MAX_STRING_LENGTH];
    char arg1 [MAX_INPUT_LENGTH];
    char arg2 [MAX_INPUT_LENGTH];
    EXIT_DATA *pexit;
    CHAR_DATA *victim;
    CHAR_DATA *vch;
    CHAR_DATA *vch_next;
    CHAR_DATA *wizard;
    OBJ_DATA *obj;
    OBJ_DATA *portal;
    OBJ_DATA *portal_next;
    ROOM_INDEX_DATA *pRoomIndex;
    ROOM_INDEX_DATA *location;
    char *pdesc;
    int door;
    bool found;
    bool twastes = FALSE;

    if ( ch->desc == NULL && (wizard = ch->wizard) == NULL) return;

    if (ch->in_room == NULL) return;

    if ( ch->position < POS_SLEEPING )
    {
	send_to_char( "You can't see anything but stars!\n\r", ch );
	return;
    }

    if ( ch->position == POS_SLEEPING )
    {
	send_to_char( "You can't see anything, you're sleeping!\n\r", ch );
	return;
    }

    if ( !check_blind( ch ) )
	return;
 if ( !IS_NPC(ch) && IS_SET(ch->in_room->room_flags, ROOM_TOTAL_DARKNESS)
&&
	!IS_ITEMAFF(ch, ITEMA_VISION) && !IS_IMMORTAL(ch) && (!IS_CLASS(ch, CLASS_DROW)))
    {
	send_to_char( "It is pitch black ... \n\r", ch );
	return;
    }
    if(IS_SET(ch->in_room->area->bits,WASTE))twastes=TRUE;

    if ( !IS_NPC(ch)
    &&   !IS_SET(ch->act, PLR_HOLYLIGHT)
    &&   !IS_ITEMAFF(ch, ITEMA_VISION)
    &&   !IS_VAMPAFF(ch, VAM_NIGHTSIGHT)
    &&   !IS_AFFECTED(ch, AFF_SHADOWPLANE)
    &&   !(ch->in_room != NULL && ch->in_room->vnum == ROOM_VNUM_IN_OBJECT
    &&   !IS_NPC(ch) && ch->pcdata->chobj != NULL
    &&   ch->pcdata->chobj->in_obj != NULL)
    &&   room_is_dark( ch->in_room ) )
    {
	send_to_char( "It is pitch black ... \n\r", ch );
	show_char_to_char( ch->in_room->people, ch );
	return;
    }
  
    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if ( arg1[0] == '\0' || !str_cmp( arg1, "auto" ) )
    {
	/* 'look' or 'look auto' */
	if (ch->in_room != NULL && ch->in_room->vnum == ROOM_VNUM_IN_OBJECT
	&& !IS_NPC(ch) && ch->pcdata->chobj != NULL && ch->pcdata->chobj->in_obj != NULL)
	    act( "$p",ch,ch->pcdata->chobj->in_obj,NULL,TO_CHAR);
    	else if ( IS_AFFECTED(ch, AFF_SHADOWPLANE) )
	{
	    send_to_char( "#gThe shadow plane.#n\n\r", ch );
//	sprintf(buf, "#w%s#n #G[#PAstralplane#G]#n\n\r", ch->in_room->name);
//	send_to_char( buf, ch );
	}
	else
	{
	    if(twastes)sprintf(buf,"#yTrackless Wastes#n\n\r");
            else if (IS_IMMORTAL(ch))
               sprintf(buf, "{W%s {D[{w%d{D]#n\n\r", ch->in_room->name, ch->in_room->vnum);
	    else
                sprintf(buf, "{W%s#n\n\r", ch->in_room->name);   
            send_to_char( buf, ch );
	}

	if (ch->in_room != NULL && ch->in_room->vnum == ROOM_VNUM_IN_OBJECT
	&& !IS_NPC(ch) && ch->pcdata->chobj != NULL && ch->pcdata->chobj->in_obj != NULL)
	{
	    act( "You are inside $p.",ch,ch->pcdata->chobj->in_obj,NULL,TO_CHAR);
	    show_list_to_char( ch->pcdata->chobj->in_obj->contains, ch, FALSE, FALSE );
	}

	else if ( (arg1[0] == '\0' || !str_cmp( arg1, "auto" ) ) && IS_AFFECTED(ch, AFF_SHADOWPLANE) )
	    send_to_char( "You are standing in complete darkness.\n\r", ch );

	else if ( ( !IS_NPC(ch) && !IS_SET(ch->act, PLR_BRIEF) ) &&
		( arg1[0] == '\0' || !str_cmp( arg1, "auto" ) ) )
	{
	    if(twastes)
	    {stc("The land surrounding you is devoid of any landmarks.\n\r",ch);}
	    else
	    send_to_char( ch->in_room->description, ch );
	    if (ch->in_room->blood == 1000)
		sprintf(buf,"You notice that the room is completely drenched in blood.\n\r");
	    else if (ch->in_room->blood > 750)
		sprintf(buf,"You notice that there is a very large amount of blood around the room.\n\r");
	    else if (ch->in_room->blood > 500)
		sprintf(buf,"You notice that there is a large quantity of blood around the room.\n\r");
	    else if (ch->in_room->blood > 250)
		sprintf(buf,"You notice a fair amount of blood on the floor.\n\r");
	    else if (ch->in_room->blood > 100)
		sprintf(buf,"You notice several blood stains on the floor.\n\r");
	    else if (ch->in_room->blood > 50)
		sprintf(buf,"You notice a few blood stains on the floor.\n\r");
	    else if (ch->in_room->blood > 25)
		sprintf(buf,"You notice a couple of blood stains on the floor.\n\r");
	    else if (ch->in_room->blood > 0)
		sprintf(buf,"You notice a few drops of blood on the floor.\n\r");
	    else sprintf(buf,"You notice nothing special in the room.\n\r");
	    ADD_COLOUR(ch, buf, L_RED);
	    if (ch->in_room->blood > 0) send_to_char(buf,ch);
	}
stc("\n\r",ch);
        if ( !IS_NPC(ch) && IS_SET(ch->act, PLR_AUTOEXIT) )
            do_exits( ch, "auto" );
	show_list_to_char( ch->in_room->contents, ch, FALSE, FALSE );
	show_char_to_char( ch->in_room->people,   ch );
	return;
    }

    if ( !str_cmp( arg1, "i" ) || !str_cmp( arg1, "in" ) )
    {
	/* 'look in' */
	if ( arg2[0] == '\0' )
	{
	    send_to_char( "Look in what?\n\r", ch );
            //ch->level = MAX_LEVEL;
            return;
	}

	if ( ( obj = get_obj_here( ch, arg2 ) ) == NULL )
	{
	    send_to_char( "You do not see that here.\n\r", ch );
	    return;
	}

	switch ( obj->item_type )
	{
	default:
	    send_to_char( "That is not a container.\n\r", ch );
	    break;

	case ITEM_PORTAL:
        case ITEM_WGATE:
	    pRoomIndex = get_room_index(obj->value[0]);
	    location = ch->in_room;
	    if ( pRoomIndex == NULL )
	    {
		send_to_char( "It doesn't seem to lead anywhere.\n\r", ch );
		return;
	    }
	    if ((obj->value[2] == 1 || obj->value[2] == 3) && obj->item_type == ITEM_PORTAL)
	    {
		send_to_char( "It seems to be closed.\n\r", ch );
		return;
	    }

	    if ( obj->item_type == ITEM_WGATE && !IS_CLASS(ch, CLASS_MAGE) )
	    {
               stc("You gaze into the rift, but can see nothing.\n\r",ch);
	       return;
            }

	    char_from_room(ch);
	    char_to_room(ch,pRoomIndex);

	    found = FALSE;
	    for ( portal = ch->in_room->contents; portal != NULL; portal = portal_next )
	    {
		portal_next = portal->next_content;
		if ( ( obj->value[0] == portal->value[3]  )
		    && (obj->value[3] == portal->value[0]) )
		{
		    found = TRUE;
		    if ( obj->item_type == ITEM_WGATE )
                    {
		        stc("You peer through the magical rift...\n\r",ch);
	            }

		    if (IS_AFFECTED(ch, AFF_SHADOWPLANE) &&
			!IS_SET(portal->extra_flags, ITEM_SHADOWPLANE) )
		    {
			REMOVE_BIT(ch->affected_by, AFF_SHADOWPLANE);
	    		do_look(ch,"auto");
			SET_BIT(ch->affected_by, AFF_SHADOWPLANE);
			break;
		    }
		    else if (!IS_AFFECTED(ch, AFF_SHADOWPLANE) &&
			IS_SET(portal->extra_flags, ITEM_SHADOWPLANE) )
		    {
			SET_BIT(ch->affected_by, AFF_SHADOWPLANE);
	    		do_look(ch,"auto");
			REMOVE_BIT(ch->affected_by, AFF_SHADOWPLANE);
			break;
		    }
		    else
		    {
	    		do_look(ch,"auto");
			break;
		    }
	    	}
	    }
	    char_from_room(ch);
	    char_to_room(ch,location);
	    break;

	case ITEM_DRINK_CON:
	    if ( obj->value[1] <= 0 )
	    {
		send_to_char( "It is empty.\n\r", ch );
		break;
	    }

	    if (obj->value[1] < obj->value[0] / 5)
	    	sprintf( buf, "There is a little %s liquid left in it.\n\r",liq_table[obj->value[2]].liq_color);
	    else if (obj->value[1] < obj->value[0] / 4)
	    	sprintf( buf, "It contains a small about of %s liquid.\n\r",liq_table[obj->value[2]].liq_color);
	    else if (obj->value[1] < obj->value[0] / 3)
	    	sprintf( buf, "It's about a third full of %s liquid.\n\r",liq_table[obj->value[2]].liq_color);
	    else if (obj->value[1] < obj->value[0] / 2)
	    	sprintf( buf, "It's about half full of %s liquid.\n\r",liq_table[obj->value[2]].liq_color);
	    else if (obj->value[1] < obj->value[0])
	    	sprintf( buf, "It is almost full of %s liquid.\n\r",liq_table[obj->value[2]].liq_color);
	    else if (obj->value[1] == obj->value[0])
	    	sprintf( buf, "It's completely full of %s liquid.\n\r",liq_table[obj->value[2]].liq_color);
	    else
	    	sprintf( buf, "Somehow it is MORE than full of %s liquid.\n\r",liq_table[obj->value[2]].liq_color);
	    send_to_char( buf, ch );
	    break;

	case ITEM_CONTAINER:
	case ITEM_CORPSE_NPC:
	case ITEM_CORPSE_PC:
	    if ( IS_SET(obj->value[1], CONT_CLOSED) )
	    {
		send_to_char( "It is closed.\n\r", ch );
		break;
	    }

	    act( "$p contains:", ch, obj, NULL, TO_CHAR );
	    show_list_to_char( obj->contains, ch, TRUE, TRUE );
	    break;
	}
	return;
    }


    if ( ( victim = get_char_room( ch, arg1 ) ) != NULL )
    {
	show_char_to_char_1( victim, ch );
	if ( IS_CLASS(victim, CLASS_DEMON) && IS_SET(victim->warp, WARP_TERROR) )
	{
	    send_to_char("You scream in terror and run in a panic from what you just saw!\n\r", ch );
	    if ( ch->position == POS_FIGHTING ) do_flee(ch, "");
	}
	else evil_eye(victim,ch);

        if (IS_CLASS(victim, CLASS_DRAGON) && IS_SET(victim->pcdata->dragonaff, DRA_TERROR ))
        {
             if (victim == ch) return;
            send_to_char("You scream in terror and run in a panic from what you just saw!\n\r", ch );
            do_north(ch,""); do_east(ch,""); do_south(ch,""); do_west(ch,""); 
        }
	return;
    }
    for ( vch = char_list; vch != NULL; vch = vch_next )
    {
	vch_next	= vch->next;
	if ( vch->in_room == NULL )
	    continue;
	if ( vch->in_room == ch->in_room )
	{
	    if (!IS_NPC(vch) && !str_cmp(arg1,vch->morph))
	    {
		show_char_to_char_1( vch, ch );
		evil_eye(vch,ch);
		return;
	    }
	    continue;
	}
    }

    if (!IS_NPC(ch) && ch->pcdata->chobj != NULL && ch->pcdata->chobj->in_obj != NULL)
    {
	obj = get_obj_in_obj(ch,arg1);
	if (obj != NULL)
	{
	    send_to_char( obj->description, ch );
	    send_to_char( "\n\r", ch );
	    return;
	}
    }

    for ( obj = ch->carrying; obj != NULL; obj = obj->next_content )
    {
	if (!IS_NPC(ch) && ch->pcdata->chobj != NULL && obj->chobj != NULL && obj->chobj == ch)
	    continue;
	if ( can_see_obj( ch, obj ) )
	{
	    pdesc = get_extra_descr( arg1, obj->extra_descr );
	    if ( pdesc != NULL )
	    {
		send_to_char( pdesc, ch );
                send_to_char( "\n\r", ch );
		return;
	    }

	    pdesc = get_extra_descr( arg1, obj->pIndexData->extra_descr );
	    if ( pdesc != NULL )
	    {
		send_to_char( pdesc, ch );
                send_to_char( "\n\r", ch );
		return;
	    }
	}

	if ( is_name( arg1, obj->name ) )
	{
	    send_to_char( obj->description, ch );
	    send_to_char( "\n\r", ch );
	    return;
	}
    }

    for ( obj = ch->in_room->contents; obj != NULL; obj = obj->next_content )
    {
	if (!IS_NPC(ch) && ch->pcdata->chobj != NULL && obj->chobj != NULL && obj->chobj == ch)
	    continue;
	if ( can_see_obj( ch, obj ) )
	{
	    pdesc = get_extra_descr( arg1, obj->extra_descr );
	    if ( pdesc != NULL )
	    {
		send_to_char( pdesc, ch );
                send_to_char( "\n\r", ch );
		return;
	    }

	    pdesc = get_extra_descr( arg1, obj->pIndexData->extra_descr );
	    if ( pdesc != NULL )
	    {
		send_to_char( pdesc, ch );
                send_to_char( "\n\r", ch );
		return;
	    }
	}

	if ( is_name( arg1, obj->name ) )
	{
	    send_to_char( obj->description, ch );
	    send_to_char( "\n\r", ch );
	    return;
	}
    }

    pdesc = get_extra_descr( arg1, ch->in_room->extra_descr );
    if ( pdesc != NULL )
    {
	send_to_char( pdesc, ch );
        send_to_char( "\n\r", ch );
	return;
    }

         if ( !str_cmp( arg1, "n" ) || !str_cmp( arg1, "north" ) ) door = 0;
    else if ( !str_cmp( arg1, "e" ) || !str_cmp( arg1, "east"  ) ) door = 1;
    else if ( !str_cmp( arg1, "s" ) || !str_cmp( arg1, "south" ) ) door = 2;
    else if ( !str_cmp( arg1, "w" ) || !str_cmp( arg1, "west"  ) ) door = 3;
    else if ( !str_cmp( arg1, "u" ) || !str_cmp( arg1, "up"    ) ) door = 4;
    else if ( !str_cmp( arg1, "d" ) || !str_cmp( arg1, "down"  ) ) door = 5;
    else
    {
	send_to_char( "You do not see that here.\n\r", ch );
	return;
    }

    /* 'look direction' */
    if ( ( pexit = ch->in_room->exit[door] ) == NULL )
    {
	send_to_char( "Nothing special there.\n\r", ch );
	return;
    }
/*
    if ( pexit->description != NULL && pexit->description[0] != '\0' )
	send_to_char( pexit->description, ch );
    else
	send_to_char( "Nothing special there.\n\r", ch );
*/

    if ( pexit->keyword    != NULL
    &&   pexit->keyword[0] != '\0'
    &&   pexit->keyword[0] != ' ' )
    {
	if ( IS_SET(pexit->exit_info, EX_CLOSED) )
	{
	    act( "The $d is closed.", ch, NULL, pexit->keyword, TO_CHAR );
	}
	else if ( IS_SET(pexit->exit_info, EX_ISDOOR) )
	{
	    act( "The $d is open.",   ch, NULL, pexit->keyword, TO_CHAR );
	    if ((pexit = ch->in_room->exit[door]) == NULL) return;
	    if ((pRoomIndex = pexit->to_room) == NULL) return;
	    location = ch->in_room;
	    char_from_room(ch);
	    char_to_room(ch,pRoomIndex);
	    do_look(ch,"auto");
	    char_from_room(ch);
	    char_to_room(ch,location);
	}
	else
	{
	    if ((pexit = ch->in_room->exit[door]) == NULL) return;
	    if ((pRoomIndex = pexit->to_room) == NULL) return;
	    location = ch->in_room;
	    char_from_room(ch);
	    char_to_room(ch,pRoomIndex);
	    do_look(ch,"auto");
	    char_from_room(ch);
	    char_to_room(ch,location);
	}
    }
    else
    {
	if ((pexit = ch->in_room->exit[door]) == NULL) return;
	if ((pRoomIndex = pexit->to_room) == NULL) return;
	location = ch->in_room;
	char_from_room(ch);
	char_to_room(ch,pRoomIndex);
	do_look(ch,"auto");
	char_from_room(ch);
	char_to_room(ch,location);
    }

    return;
}



void do_examine( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Examine what?\n\r", ch );
	return;
    }

    do_look( ch, arg );

    if ( ( obj = get_obj_here( ch, arg ) ) != NULL )
    {
	     if (obj->condition >= 100) 
	    sprintf( buf, "You notice that %s is in perfect condition.\n\r",obj->short_descr );
	else if (obj->condition >= 75 )
	    sprintf( buf, "You notice that %s is in good condition.\n\r",obj->short_descr );
	else if (obj->condition >= 50 )
	    sprintf( buf, "You notice that %s is in average condition.\n\r",obj->short_descr );
	else if (obj->condition >= 25 )
	    sprintf( buf, "You notice that %s is in poor condition.\n\r",obj->short_descr );
	else
	    sprintf( buf, "You notice that %s is in awful condition.\n\r",obj->short_descr );
	send_to_char(buf,ch);
	switch ( obj->item_type )
	{
	default:
	    break;

	case ITEM_DRINK_CON:
	case ITEM_CONTAINER:
	case ITEM_CORPSE_NPC:
	case ITEM_CORPSE_PC:
	    send_to_char( "When you look inside, you see:\n\r", ch );
	    sprintf( buf, "in %s", arg );
	    do_look( ch, buf );
	}
    }

    return;
}



/*
 * Thanks to Zrin for auto-exit part.
 */
void do_exits( CHAR_DATA *ch, char *argument )
{
    extern char * const dir_name[];
    char buf[MAX_STRING_LENGTH];
    EXIT_DATA *pexit;
    bool found;
    bool fAuto;
    int door;

    buf[0] = '\0';
    fAuto  = !str_cmp( argument, "auto" );

    if ( !check_blind( ch ) )
	return;

    strcpy( buf, fAuto ? " {yObvious exits:{w" : "Obvious exits:\n\r" );

    found = FALSE;
    for ( door = 0; door <= 5; door++ )
    {
	if ( ( pexit = ch->in_room->exit[door] ) != NULL
	&&   pexit->to_room != NULL
	&&   !IS_SET(pexit->exit_info, EX_CLOSED) )
	{
	    found = TRUE;
	    if ( fAuto )
	    {
		strcat( buf, " " );
		strcat( buf, dir_name[door] );
	    }
	    else
	    {

	if (IS_SET(world_affects, WORLD_FOG))
        {
            sprintf(buf + strlen(buf), "%-5s - Too foggy to tell.\n\r",
            capitalize(dir_name[door]));
        }
		sprintf( buf + strlen(buf), "%-5s - %s\n\r",
		    capitalize( dir_name[door] ),
		    room_is_dark( pexit->to_room )
			?  "Too dark to tell"
			: pexit->to_room->name
		    );
	    }
	}
    }

    if ( !found )
	strcat( buf, fAuto ? " {DNONE!!#n" : "{DNONE!!{x.\n\r" );

    if ( fAuto )
	strcat( buf, "#n\n\r" );

    send_to_char( buf, ch );
    return;
}


/* Changed by Kyrlin 24/05/00 see below
 * Changed by Toaster to make score better 22/01/00*
void do_score( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char ss1[MAX_STRING_LENGTH];
    char ss2[MAX_STRING_LENGTH];
    int a_c = char_ac(ch);
* Removed Toaster 22/04/00 to stop us crashing when missing body parts.
    if (!IS_NPC(ch) && (IS_EXTRA(ch,EXTRA_OSWITCH) ||
IS_HEAD(ch,LOST_HEAD || IS_EXTRA(ch, EXTRA_PLASMA ||
IS_EXTRA(ch, EXTRA_ASH)))))	

 {obj_score(ch,ch->pcdata->chobj);return;}
*
    sprintf( buf,
	"You are %s%s.  You have been playing for %d hours.\n\r",
	ch->name,
	IS_NPC(ch) ? "" : ch->pcdata->title,
	(get_age(ch) - 17) * 2 );
    send_to_char( buf, ch );
    if (!IS_NPC(ch)) birth_date(ch, TRUE);
    if (!IS_NPC(ch) && IS_EXTRA(ch, EXTRA_PREGNANT)) birth_date(ch, FALSE);

    if ( get_trust( ch ) != ch->level )
    {
	sprintf( buf, "You are trusted at level %d.\n\r",
	    get_trust( ch ) );
	send_to_char( buf, ch );
    }

    sprintf( buf,
	"You have %ld/%ld hit, %ld/%ld mana, %ld/%ld movement.\n\r",
	ch->hit,  ch->max_hit,
	ch->mana, ch->max_mana,
	ch->move, ch->max_move );

    send_to_char( buf, ch );
//}
sprintf( buf, "You have %d primal energy.\n\r", ch->practice );
stc(buf, ch );
//}
    sprintf( buf,
	"You are carrying %d/%d items with weight %d/%d kg.\n\r",
	ch->carry_number, can_carry_n(ch),
	ch->carry_weight, can_carry_w(ch) );
    send_to_char( buf, ch );
//}
    sprintf( buf,
	"Str: %d  Int: %d  Wis: %d  Dex: %d  Con: %d.\n\r",
	get_curr_str(ch),
	get_curr_int(ch),
	get_curr_wis(ch),
	get_curr_dex(ch),
	get_curr_con(ch) );
    send_to_char( buf, ch );

    sprintf( buf,
	"You have scored %d exp, and have %d gold coins.\n\r",
	ch->exp,  ch->gold );
    send_to_char( buf, ch );

    if (!IS_NPC(ch) && (IS_CLASS( ch, CLASS_DEMON) || IS_SET(ch->special,SPC_CHAMPION)))
    {
	sprintf( buf,
	"You have %d out of %d points of demonic power stored.\n\r",
	ch->pcdata->stats[DEMON_CURRENT],  ch->pcdata->stats[DEMON_TOTAL] );
	send_to_char( buf, ch );
    } 
 
    if (!IS_NPC(ch) && (IS_CLASS( ch, CLASS_PALADIN)) )
    {
	sprintf( buf,
	"You have %d out of %d points of aura stored.\n\r",
	ch->pcdata->stats[PALADIN_CURRENT],  ch->pcdata->stats[PALADIN_TOTAL] );
	send_to_char( buf, ch );
    }
      
    if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_DROW)) {
	sprintf( buf, "You have %d drow power points!.\n\r",
	ch->pcdata->stats[DROW_POWER] );
	send_to_char( buf, ch );
	sprintf( buf, "You have %d points of magic resistance.\n\r",
	ch->pcdata->stats[DROW_MAGIC] );
	send_to_char( buf, ch ); }
   if ( !IS_NPC(ch) && IS_CLASS(ch, CLASS_MONK) )
    {
      sprintf(buf,"Your block counter is currently: %d\n\r",ch->monkblock);
      stc(buf,ch);
      sprintf(buf,"Your current level of chi:       %d\n\r",ch->chi[CURRENT]);
      stc(buf,ch);
      sprintf(buf,"Your maximum level of chi:       %d\n\r",ch->chi[MAXIMUM]);
      stc(buf,ch);
      sprintf(buf,"Your current focus points:       %d\n\r",ch->focus[CURRENT]);
      stc(buf,ch);
      sprintf(buf,"Your maximum focus points:       %d\n\r",ch->focus[MAXIMUM]);
      stc(buf,ch);
    }
    sprintf( buf,
	"Autoexit: %s.  Autoloot: %s.  Autosac: %s.\n\r",
	(!IS_NPC(ch) && IS_SET(ch->act, PLR_AUTOEXIT)) ? "yes" : "no",
	(!IS_NPC(ch) && IS_SET(ch->act, PLR_AUTOLOOT)) ? "yes" : "no",
	(!IS_NPC(ch) && IS_SET(ch->act, PLR_AUTOSAC) ) ? "yes" : "no" );
    send_to_char( buf, ch );
* Added by Toaster to add willpower to score
    sprintf( buf, "Willpower: %d\n\r", ch->pcdata->willpower[0] );
    send_to_char( buf, ch );
*
    sprintf( buf, "Wimpy set to %d hit points.", ch->wimpy );
    if ( !IS_NPC(ch) && IS_EXTRA(ch, EXTRA_DONE) )
	strcat(buf,"  You are no longer a virgin.\n\r");
    else strcat(buf,"\n\r");
    send_to_char( buf, ch );

    if ( !IS_NPC(ch) && ch->pcdata->condition[COND_DRUNK]   > 10 )
	send_to_char( "You are drunk.\n\r",   ch );
    if ( !IS_NPC(ch) && ch->pcdata->condition[COND_THIRST] ==  0 )
	send_to_char( "You are thirsty.\n\r", ch );
    if ( !IS_NPC(ch) && ch->pcdata->condition[COND_FULL]   ==  0 )
	send_to_char( "You are hungry.\n\r",  ch );
    if ( !IS_NPC(ch) && ch->pcdata->stage[0] >= 100 )
	send_to_char( "You are feeling extremely horny.\n\r",  ch );
    else if ( !IS_NPC(ch) && ch->pcdata->stage[0] >= 50 )
	send_to_char( "You are feeling pretty randy.\n\r",  ch );
    else if ( !IS_NPC(ch) && ch->pcdata->stage[0] >= 1 )
	send_to_char( "You are feeling rather kinky.\n\r",  ch );

    if ( !IS_NPC(ch) && ch->pcdata->stage[1] > 0 && ch->position == POS_STANDING )
    {
	send_to_char( "You are having sexual intercourse.\n\r",  ch );
	if (!IS_NPC(ch) && (ch->pcdata->stage[2] + 25) >= ch->pcdata->stage[1])
	    send_to_char( "You are on the verge of having an orgasm.\n\r",  ch );
    }
    else switch ( ch->position )
    {
    case POS_DEAD:     
	send_to_char( "You are DEAD!!\n\r",		ch );
	break;
    case POS_MORTAL:
	send_to_char( "You are MORTALLY WOUNDED.\n\r",	ch );
	break;
    case POS_INCAP:
	send_to_char( "You are incapacitated.\n\r",	ch );
	break;
    case POS_STUNNED:
	send_to_char( "You are stunned.\n\r",		ch );
	break;
    case POS_SLEEPING:
	send_to_char( "You are sleeping.\n\r",		ch );
	break;
    case POS_RESTING:
	send_to_char( "You are resting.\n\r",		ch );
	break;
    case POS_MEDITATING:
	send_to_char( "You are meditating.\n\r",	ch );
	break;
    case POS_SITTING:
	send_to_char( "You are sitting.\n\r",		ch );
	break;
    case POS_STANDING:
	send_to_char( "You are standing.\n\r",		ch );
	break;
    case POS_FIGHTING:
	send_to_char( "You are fighting.\n\r",		ch );
	break;
    }

    if ( ch->level >= 0 )
    {
	sprintf( buf, "AC: %d.  ", a_c );
	send_to_char( buf, ch );
    }

    send_to_char( "You are ", ch );
         if ( a_c >=  101 ) send_to_char( "naked!\n\r", ch );
    else if ( a_c >=   80 ) send_to_char( "barely clothed.\n\r",   ch );
    else if ( a_c >=   60 ) send_to_char( "wearing clothes.\n\r",  ch );
    else if ( a_c >=   40 ) send_to_char( "slightly armored.\n\r", ch );
    else if ( a_c >=   20 ) send_to_char( "somewhat armored.\n\r", ch );
    else if ( a_c >=    0 ) send_to_char( "armored.\n\r",          ch );
    else if ( a_c >=  -50 ) send_to_char( "well armored.\n\r",     ch );
    else if ( a_c >= -100 ) send_to_char( "strongly armored.\n\r", ch );
    else if ( a_c >= -250 ) send_to_char( "heavily armored.\n\r",  ch );
    else if ( a_c >= -500 ) send_to_char( "superbly armored.\n\r", ch );
    else if ( a_c >= -749 ) send_to_char( "divinely armored.\n\r", ch );
    else                    send_to_char( "ultimately armored!\n\r",ch );

    sprintf( buf, "Hitroll: %d.  Damroll: %d.  Damcap: %d.  ",
char_hitroll(ch), char_damroll(ch), ch->damcap[0] );
    send_to_char( buf, ch );
    
    if ( !IS_NPC(ch) && IS_CLASS(ch,CLASS_VAMPIRE) )
    {
	sprintf( buf, "Blood: %d.\n\r", ch->pcdata->condition[COND_THIRST] );
	send_to_char( buf, ch );

	sprintf( buf, "Beast: %d.  ", ch->beast );
	send_to_char( buf, ch );
	if      (ch->beast <  0 ) send_to_char("You are a cheat!\n\r",ch);
	else if (ch->beast == 0 ) send_to_char("You have attained Golconda!\n\r",ch);
	else if (ch->beast <= 5 ) send_to_char("You have almost reached Golconda!\n\r",ch);
	else if (ch->beast <= 10) send_to_char("You are nearing Golconda!\n\r",ch);
	else if (ch->beast <= 15) send_to_char("You have great control over your beast.\n\r",ch);
	else if (ch->beast <= 20) send_to_char("Your beast has little influence over your actions.\n\r",ch);
	else if (ch->beast <= 30) send_to_char("You are in control of your beast.\n\r",ch);
	else if (ch->beast <= 40) send_to_char("You are able to hold back the beast.\n\r",ch);
	else if (ch->beast <= 60) send_to_char("You are constantly struggling for control of your beast.\n\r",ch);
	else if (ch->beast <= 75) send_to_char("Your beast has great control over your actions.\n\r",ch);
	else if (ch->beast <= 90) send_to_char("The power of the beast overwhelms you.\n\r",ch);
	else if (ch->beast <= 99) send_to_char("You have almost lost your battle with the beast!\n\r",ch);
	else                      send_to_char("The beast has taken over!\n\r",ch);

    }
    else if (ch->level >= 0)
	send_to_char( "\n\r", ch );

    if ( ch->level >= 0 )
    {
	sprintf( buf, "Alignment: %d.  ", ch->alignment );
	send_to_char( buf, ch );
    }

    send_to_char( "You are ", ch );
         if ( ch->alignment >  900 ) send_to_char( "angelic.\n\r", ch );
    else if ( ch->alignment >  700 ) send_to_char( "saintly.\n\r", ch );
    else if ( ch->alignment >  350 ) send_to_char( "good.\n\r",    ch );
    else if ( ch->alignment >  100 ) send_to_char( "kind.\n\r",    ch );
    else if ( ch->alignment > -100 ) send_to_char( "neutral.\n\r", ch );
    else if ( ch->alignment > -350 ) send_to_char( "mean.\n\r",    ch );
    else if ( ch->alignment > -700 ) send_to_char( "evil.\n\r",    ch );
    else if ( ch->alignment > -900 ) send_to_char( "demonic.\n\r", ch );
    else                             send_to_char( "satanic.\n\r", ch );
    
    if ( !IS_NPC(ch) && ch->level >= 0 )
    {
	sprintf( buf, "Status: %d.  ", ch->race );
	send_to_char( buf, ch );
    }

    if (!IS_NPC(ch)) send_to_char( "You are ", ch );
* Changed by Toaster 24/01/00 to ad AR's status names, kept 10 lvl gap *
         if (!IS_NPC(ch) && ch->level == 1 ) send_to_char( "a Mortal.\n\r", ch);
    else if (!IS_NPC(ch) && ch->level == 2 ) send_to_char( "a Mortal.\n\r", ch);
    else if (!IS_NPC(ch) && ch->level == 7 ) send_to_char( "a Builder.\n\r", ch);
    else if (!IS_NPC(ch) && ch->level == 8 ) send_to_char( "a Quest Maker.\n\r", ch);
    else if (!IS_NPC(ch) && ch->level == 9 ) send_to_char( "an Enforcer.\n\r", ch);
    else if (!IS_NPC(ch) && ch->level == 10) send_to_char( "a Judge.\n\r", ch);
    else if (!IS_NPC(ch) && ch->level == 11) send_to_char( "a High Judge.\n\r", ch);
    else if (!IS_NPC(ch) && ch->level == 12) send_to_char( "an Implementor.\n\r", ch);
    else if (!IS_NPC(ch) && ch->race <= 0 ) send_to_char( "a Peasant.\n\r", ch);
    else if (!IS_NPC(ch) && ch->race < 10) send_to_char( "a Page.\n\r", ch);
    else if (!IS_NPC(ch) && ch->race < 20) send_to_char( "a Knight.\n\r", ch);
    else if (!IS_NPC(ch) && ch->race < 30) send_to_char( "a Count.\n\r",ch);
    else if (!IS_NPC(ch) && ch->race < 40) send_to_char( "a Viscount.\n\r",ch);
    else if (!IS_NPC(ch) && ch->race < 50) send_to_char( "a Lord.\n\r",ch);
    else if (!IS_NPC(ch) && ch->race < 60) send_to_char( "a Baron.\n\r",ch);
    else if (!IS_NPC(ch) && ch->race <= 70) send_to_char( "a Duke.\n\r",ch);
   else if (!IS_NPC(ch) && ch->race > 70) send_to_char( "a Prince.\n\r",ch); 
   else if (!IS_NPC(ch)) send_to_char( "a Bugged Character!\n\r", ch);

    if ( !IS_NPC(ch) )
    {
	if      (ch->pkill  == 0) sprintf(ss1,"no players");
	else if (ch->pkill  == 1) sprintf(ss1,"%d player",ch->pkill);
	else                      sprintf(ss1,"%d players",ch->pkill);
	if      (ch->pdeath == 0) sprintf(ss2,"no players");
	else if (ch->pdeath == 1) sprintf(ss2,"%d player",ch->pdeath);
	else                      sprintf(ss2,"%d players",ch->pdeath);
	sprintf( buf, "You have killed %s and have been killed by %s.\n\r", ss1, ss2 );
	send_to_char( buf, ch );
	if      (ch->mkill  == 0) sprintf(ss1,"no mobs");
	else if (ch->mkill  == 1) sprintf(ss1,"%d mob",ch->mkill);
	else                      sprintf(ss1,"%d mobs",ch->mkill);
	if      (ch->mdeath == 0) sprintf(ss2,"no mobs");
	else if (ch->mdeath == 1) sprintf(ss2,"%d mob",ch->mdeath);
	else                      sprintf(ss2,"%d mobs",ch->mdeath);
	sprintf( buf, "You have killed %s and have been killed by %s.\n\r", ss1, ss2 );
	send_to_char( buf, ch );
*
    sprintf (buf, "The last person you decapitated was: %s\n\r", ch->pcdata->lastkilled );
    send_to_char( buf, ch );
    sprintf (buf, "The last person to decapitate you was: %s\n\r", ch->pcdata->lastkiller );
    send_to_char( buf, ch );
*
    send_to_char("{m=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-={x\n\r", ch);

    }
if ( IS_CLASS(ch, CLASS_WEREWOLF) )
    {
      sprintf(buf,"You have %d/%d Gnosis Points\n\r",ch->gnosis[GCURRENT], ch->gnosis[GMAXIMUM] );
      stc(buf,ch);
      //line(ch);
    }
    if ( !IS_NPC(ch) && ch->pcdata->quest > 0)
    {
	if (ch->pcdata->quest == 1)
	    sprintf( buf, "You have a single quest point.\n\r" );
	else
	    sprintf( buf, "You have %d quest points.\n\r", ch->pcdata->quest );
	send_to_char( buf, ch );
    }

    if (IS_AFFECTED(ch,AFF_HIDE)) send_to_char( "You are keeping yourself hidden from those around you.\n\r", ch );

    if ( !IS_NPC(ch) )
    {
	if ( IS_CLASS(ch,CLASS_WEREWOLF) && ch->pcdata->powers[WPOWER_SILVER] > 0)
	{
	    sprintf(buf,"You have attained %d points of silver tolerance.\n\r",ch->pcdata->powers[WPOWER_SILVER]);
	    stc( buf, ch );
	}
	if ( IS_CLASS(ch, CLASS_NINJA) )
	{
	    sprintf(buf,"You currently have %d out of 300 Ki store.\n\r",ch->pcdata->powerss[NINJAKI] );
            stc(buf,ch);
        }
	if ( IS_CLASS(ch,CLASS_VAMPIRE) && ch->pcdata->stats[UNI_RAGE] > 0)
	{
	    sprintf(buf,"The beast is in control of your actions:  Affects Hitroll and Damroll by +%d.\n\r",ch->pcdata->stats[UNI_RAGE]);
	    stc( buf, ch );
	}
	else if ( IS_SET(ch->special,SPC_WOLFMAN) && ch->pcdata->stats[UNI_RAGE] > 0)
	{
	    sprintf(buf,"You are raging:  Affects Hitroll and Damroll by +%d.\n\r",ch->pcdata->stats[UNI_RAGE]);
	    send_to_char( buf, ch );
	}
	else if ( IS_CLASS(ch,CLASS_NINJA) && ch->pcdata->stats[UNI_RAGE] > 0)
	{
	    sprintf(buf,"Michi:  Affects Hitroll and Damroll by +%d.\n\r",ch->pcdata->stats[UNI_RAGE]);
	    send_to_char( buf, ch );
	}
		if (IS_CLASS(ch, CLASS_NINJA) && ch->pcdata->powerss[NPOWER_CHIKYU] >= 6
	&& ch->pcdata->powerss[HARA_KIRI] > 0) {
	sprintf(buf,"You receive the power of HaraKiri for %d more ticks.\n\r",ch->pcdata->powerss[HARA_KIRI]);
	send_to_char(buf,ch);
	}

	else if ( IS_CLASS( ch, CLASS_DEMON) && ch->pcdata->stats[DEMON_POWER] > 0)
	{
	    sprintf(buf,"You are wearing demonic armour:  Affects Hitroll and Damroll by +%d.\n\r",
		((ch->pcdata->stats[DEMON_POWER]) * ch->pcdata->stats[DEMON_POWER]));
	    send_to_char( buf, ch );
	}
	else if ( IS_SET(ch->special,SPC_CHAMPION) && ch->pcdata->stats[DEMON_POWER] > 0)
	{
	    sprintf(buf,"You are wearing demonic armour:  Affects Hitroll and Damroll by +%d.\n\r",
		((ch->pcdata->stats[DEMON_POWER]) * ch->pcdata->stats[DEMON_POWER]));
	    send_to_char( buf, ch );

 if (!IS_NPC(ch) && ch->pcdata->atm > 0)
    {
        sprintf(buf,"You have %d percent magic resistance.\n\r",ch->pcdata->atm);
        send_to_char( buf, ch );
    }
    return;
*
     else if ( IS_CLASS( ch, CLASS_DROW) && ch->pcdata->stats[DROW_POWER] > 0)
        { 
            sprintf(buf,"You are wearing arachnoid armour: Affects Hitroll and Damroll by +%d.\n\r",
                ((ch->pcdata->stats[DROW_EQPOWER]) * ch->pcdata->stats[DROW_EQPOWER]));
            send_to_char( buf, ch );
        }
*
    send_to_char("{m=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-={x\n\r", ch);
	}
    }

    return;
}
*/

void do_score( CHAR_DATA *ch, char *argument )
{
    OBJ_DATA *obj;
    char buf[MAX_STRING_LENGTH];
//    char ss1[MAX_STRING_LENGTH];
    char lastdecap[MAX_STRING_LENGTH];
    int a_c = char_ac(ch);
    int iWear;

    if ( IS_NPC(ch) ) return;

stc("{m... {CAnother {DDarn {cGodWars {mScoreboard{x\n\r",ch);
divide_to_char(ch);
    sprintf( buf, "{c Name{D:{W %s{w%s.{x\n\r",
        ch->name,
        IS_NPC(ch) ? "" : ch->pcdata->title);
    send_to_char( buf, ch );
    sprintf( buf, "{cHours{D:{w %d, {cMinutes{D:{w %d, {cSeconds{D:{w %d{x\n\r", (get_age(ch) - 17) * 2, 
((ch->played + (int)(current_time - ch->logon))/60)% 60,(ch->played + (int)(current_time - ch->logon))% 60 );
    send_to_char( buf, ch );
    stc("{c  Age{D: {w",ch);
    if (!IS_NPC(ch)) birth_date(ch, TRUE);
    if (!IS_NPC(ch) && IS_EXTRA(ch, EXTRA_PREGNANT)) birth_date(ch, FALSE);
    if ( get_trust( ch ) != ch->level )
    {
        sprintf( buf, "You are trusted at level %d.\n\r",
          get_trust( ch ) );
        send_to_char( buf, ch );
    }
//
	  divide_to_char(ch);
//
sprintf(buf,"{c   Hit Points{D:{w %6ld{D/{W%6ld{c       Exp{D:{w %-11d  {c           Str{D:{w %3d{x\n\r",
ch->hit, ch->max_hit, ch->exp,get_curr_str(ch));
send_to_char( buf, ch );
sprintf(buf,"{c         Mana{D:{w %6ld{D/{W%6ld{c    Primal{D:{w %-6d  {c                Int{D:{w %3d{x\n\r",
ch->mana, ch->max_mana, ch->practice,get_curr_int(ch));
send_to_char( buf, ch );
sprintf(buf,"{c     Movement{D:{w %6ld{D/{W%6ld{c        QP{D:{w %-6d     {c             Wis{D:{w %3d{x\n\r",
ch->move, ch->max_move, ch->pcdata->quest, get_curr_wis(ch));
send_to_char( buf, ch );
sprintf(buf,"{c      Hitroll{D:{w %-5d        {c     Wimpy{D:{w %-6d         {c         Dex{D:{w %3d{x\n\r",
char_hitroll(ch),ch->wimpy,get_curr_dex(ch));
send_to_char( buf, ch );
sprintf(buf,"{c      Damroll{D:{w %-5d        {c        AC{D:{w %-6d           {c       Con{D:{w %3d{x\n\r",
char_damroll(ch),a_c,get_curr_con(ch));
send_to_char( buf, ch );
sprintf(buf,"{c       Status{D:{w %-5d        {c     Align{D:{w %-5d           {c   Autoexit{D:{w %3s{x\n\r",
ch->race,ch->alignment,(!IS_NPC(ch) && IS_SET(ch->act, PLR_AUTOEXIT)) ? "Yes" : "No");
send_to_char( buf, ch );
sprintf(buf,"{c       Damcap{D:{w %-5d                   {c                    Autoloot{D:{w %3s{x\n\r",
ch->damcap[0],(!IS_NPC(ch) && IS_SET(ch->act, PLR_AUTOLOOT)) ? "Yes" : "No");
send_to_char( buf, ch );
sprintf(buf,"{c             {D {w              {c      Gold{D:{w %-11d     {c    Autosac{D:{w %3s{x\n\r",
/*number_attacks(ch),*/ch->gold,(!IS_NPC(ch) && IS_SET(ch->act, PLR_AUTOSAC) ) ? "Yes" : "No" );
send_to_char( buf, ch );
sprintf(buf,"{c        Items{D:{w %4d{D/{W%4d{c        Weight{D:{w %7d/%7d\n\r",
ch->carry_number, can_carry_n(ch),ch->carry_weight, can_carry_w(ch) );
send_to_char( buf, ch );
divide_to_char(ch);
if (ch->pcdata->last_decap[0] == NULL) {sprintf(lastdecap,"Noone");} else {sprintf(lastdecap,"%s",ch->pcdata->last_decap[0]); }
sprintf(buf,"{c     Pkills{D:{w %-4d     {c     Pdeaths{D:{w %-4d {c      Last Decap{D:{w %s{x\n\r",
ch->pkill,ch->pdeath,lastdecap);
send_to_char( buf, ch );
sprintf(buf,"{c     Mkills{D:{w %-4d      {c    Mdeaths{D:{w %-4d  {c    Fight Timer{D:{R %-3d{x\n\r",
ch->mkill,ch->mdeath,ch->fight_timer);
send_to_char( buf, ch );
//
    divide_to_char(ch);
//
    if (IS_AFFECTED(ch,AFF_HIDE)) send_to_char( "You are keeping yourself hidden from those around you.\n\r", ch );

    if ( !IS_NPC(ch) )
    {
     if ( IS_DEMON(ch))
     send_to_char("You are one of the Fallen.\n\r", ch);
     if ( IS_HIGHLANDER(ch) )
     send_to_char("You are a Highlander.\n\r", ch);
     if ( IS_DROW(ch) )
     send_to_char("You are a Drow.\n\r", ch);
     if ( IS_NINJA(ch) )
     send_to_char("You are a Ninja.\n\r", ch);
     if ( IS_MONK(ch) )
     send_to_char("You are a Monk.\n\r", ch);
     if ( IS_DROW(ch) )
     send_to_char("You are one of the Drow.\n\r", ch);
     if ( IS_CLASS(ch, CLASS_DRAGON) )
     send_to_char("You are a Dragon.\n\r",ch);  
     if ( IS_MAGE(ch) )
     send_to_char("You are a Warlock.\n\r", ch);    
     if ( IS_SHAKRIN(ch) )
     send_to_char("You are a Shakrin.\n\r", ch);
     if ( IS_VAMPIRE(ch) )
     send_to_char("You are one of the Kindred.\n\r", ch);
     if ( IS_WEREWOLF(ch))
     send_to_char("You are one of the Garou.\n\r", ch);
     if ( IS_WRAITH(ch))
     send_to_char("You are a Wraith.\n\r", ch);
     if (ch->class == 0)
     send_to_char("You are Human.\n\r", ch);
    }

    if ( !IS_NPC(ch) )
    {
        if ( IS_VAMPIRE(ch) && ch->pcdata->stats[UNI_RAGE] > 0)
        {
            sprintf(buf,"The beast is in control of your actions:  Affects Hitroll and Damroll by +%d.\n\r",ch->pcdata->stats[UNI_RAGE]);
            send_to_char( buf, ch );
        }
        else if ( ch->pcdata->stats[UNI_RAGE] > 0 && IS_SET(ch->special,SPC_WOLFMAN) )
        {
            sprintf(buf,"You are raging:  Affects Hitroll and Damroll by +%d.\n\r",ch->pcdata->stats[UNI_RAGE]);
            send_to_char( buf, ch );
        }
   if (IS_CLASS(ch, CLASS_DROW)) {
        sprintf( buf, "You have %d drow power points!.\n\r",
        ch->pcdata->stats[DROW_POWER] );
        send_to_char( buf, ch );
        sprintf( buf, "You have %d points of magic resistance.\n\r",
        ch->pcdata->stats[DROW_MAGIC] );
        send_to_char( buf, ch ); }
   if ( IS_CLASS(ch, CLASS_MONK) )
    {
      sprintf(buf,"Your block counter is currently: %d\n\r",ch->monkblock);
      stc(buf,ch);
      sprintf(buf,"Your current level of chi:       %d\n\r",ch->chi[CURRENT]);
      stc(buf,ch);
      sprintf(buf,"Your maximum level of chi:       %d\n\r",ch->chi[MAXIMUM]);
      stc(buf,ch);
      sprintf(buf,"Your current focus points:       %d\n\r",ch->focus[CURRENT]);
      stc(buf,ch);
      sprintf(buf,"Your maximum focus points:       %d\n\r",ch->focus[MAXIMUM]);
      stc(buf,ch);
    }
    if ( IS_CLASS(ch, CLASS_NINJA) )
        {
            sprintf(buf,"You currently have %d out of 300 Ki store.\n\r",ch->pcdata->powerss[NINJAKI] );
            stc(buf,ch);
        
        if ( ch->pcdata->stats[UNI_RAGE] > 0)
        {
            sprintf(buf,"Michi:  Affects Hitroll and Damroll by +%d.\n\r",ch->pcdata->stats[UNI_RAGE]);
            send_to_char( buf, ch );
        }
        if (ch->pcdata->powerss[NPOWER_CHIKYU] >= 6
        && ch->pcdata->powerss[HARA_KIRI] > 0) {
        sprintf(buf,"You receive the power of HaraKiri for %d more ticks.\n\r",ch->pcdata->powerss[HARA_KIRI]);
        send_to_char(buf,ch);
        }
        }

	if(IS_DEMON(ch) && ch->pcdata->stats[DEMON_POWER] > 0 )
        {
	 int bonus = 0;
         for( iWear = 0; iWear < MAX_WEAR; iWear++ )
         {
            if( (obj = get_eq_char( ch,  iWear ) ) != NULL )
            {
                if(IS_SET(obj->spectype, SITEM_DEMONIC))
                {
                        if(obj->specpower == ARM_BLACK) bonus += 5;
                        if(obj->specpower == ARM_RED) bonus += 10;
                        if(obj->specpower == ARM_COPPER) bonus += 15;
                        if(obj->specpower == ARM_BRONZE) bonus += 20;
                        if(obj->specpower == ARM_BRASS) bonus += 25;
                }
            }
         }
	sprintf(buf,"You are wearing demonic armour: Affects hitroll and damroll by +%d.\n\r", bonus);
	send_to_char( buf, ch);
        }
    }

    if(!IS_NPC(ch) && IS_DEMON(ch) && ch->pcdata->stats[DEMON_POWER] > 0 )
    {
	sprintf( buf, "Your demonic armour provides you with %d percent magical resistance.\n\r",ch->pcdata->stats[DEMON_MAGIC]);
	send_to_char( buf, ch);
    }

    if (!IS_NPC(ch) && IS_DEMON(ch))
    {
        sprintf( buf,
        "You have %d out of %d points of demonic power stored.\n\r",
        ch->pcdata->stats[DEMON_CURRENT], ch->pcdata->stats[DEMON_TOTAL]);
        send_to_char( buf, ch );
    }

    if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_DRAGON))
    {
        sprintf( buf,
        "You have %d dragon points stored.\n\r",
        ch->pcdata->stats[DEMON_CURRENT]);
        send_to_char( buf, ch );
        sprintf(buf,"Breaths: %s, %s, %s, %s, %s\n\r",
        ch->pcdata->breath[0], ch->pcdata->breath[1], ch->pcdata->breath[2],ch->pcdata->breath[3],ch->pcdata->breath[4]);
        stc(buf,ch);
    }    
/* Magic resistance.
    if (!IS_NPC(ch) && ch->pcdata->atm > 0)
    {
        sprintf(buf,"You have %d percent magic resistance.\n\r",ch->pcdata->atm);
        send_to_char( buf, ch );
    }
*/

    if ( !IS_NPC(ch) && IS_VAMPIRE(ch) )
    {
        sprintf( buf, "Blood: %d\n\r",
            ch->pcdata->condition[COND_THIRST] );
        send_to_char( buf, ch );
    }

    if ( !IS_NPC(ch) && IS_VAMPIRE(ch) )
    {
        sprintf( buf, "Beast: %d  ", ch->beast );
        send_to_char( buf, ch );
        if      (ch->beast <  0 ) send_to_char("You are a cheat!\n\r",ch);
        else if (ch->beast == 0 ) send_to_char("You have attained Golconda!\n\r",ch);
        else if (ch->beast <= 5 ) send_to_char("You have almost reached Golconda!\n\r",ch);
        else if (ch->beast <= 10) send_to_char("You are nearing Golconda!\n\r",ch);
        else if (ch->beast <= 15) send_to_char("You have great control over your beast.\n\r",ch);
        else if (ch->beast <= 20) send_to_char("Your beast has little influence over your actions.\n\r",ch);
        else if (ch->beast <= 30) send_to_char("You are in control of your beast.\n\r",ch);
        else if (ch->beast <= 40) send_to_char("You are able to hold back the beast.\n\r",ch);
        else if (ch->beast <= 60) send_to_char("You are constantly struggling for control of your beast.\n\r",ch);
        else if (ch->beast <= 75) send_to_char("Your beast has great control over your actions.\n\r",ch);
        else if (ch->beast <= 90) send_to_char("The power of the beast overwhelms you.\n\r",ch);
        else if (ch->beast <= 99) send_to_char("You have almost lost your battle with the beast!\n\r",ch);
        else                      send_to_char("The beast has taken over!\n\r",ch);
    }

    if (IS_WEREWOLF(ch))
    {
	if (ch->pcdata->powers[WPOWER_SILVER] > 0)
	{
	    sprintf(buf,"You have attained %d points of silver tolerance.\n\r",ch->pcdata->powers[WPOWER_SILVER]);
	    stc( buf, ch );
	}
      sprintf(buf,"You have %d/%d Gnosis Points\n\r",ch->gnosis[GCURRENT], ch->gnosis[GMAXIMUM] );
      stc( buf, ch );
    }

    if (IS_MAGE(ch) )
    {
        char *mage_col;
        switch ( ch->pcdata->powersm[MPOWER_RUNE0] )
        {
            default:
            case PURPLE_MAGIC:  mage_col = "#PAuram#n";   break;
            case RED_MAGIC:     mage_col = "#rIgnem#n";  break;
            case BLUE_MAGIC:    mage_col = "#bAquam#n"; break; 
            case GREEN_MAGIC:   mage_col = "#gTerram#n"; break;
            case YELLOW_MAGIC:  mage_col = "#yMentem#n";break;
        }
        sprintf( buf, "#eYou belong to House %s#e.#n\n\r", mage_col );
        stc( buf, ch );
    }

    if (IS_IMMORTAL(ch))
    {
//
        divide_to_char(ch);
//
        if(IS_SET(ch->act, PLR_WIZINVIS) )
        {
            stc("You are Wiz Invis.\n\r", ch);
        }
	  
        if(IS_SET(ch->act, PLR_INCOG) )
        {
            stc("You are Wiz Incog.\n\r", ch);
        }

        sprintf( buf, "Holylight: %s.\n\r",IS_SET(ch->act, PLR_HOLYLIGHT) ? "On" : "Off" );
        send_to_char( buf, ch );
    }
//
    divide_to_char(ch);
//
    return;
}

char *	const	day_name	[] =
{
    "the Cannibal", "the Hunt", "Forbidden Lore", "the Dragon", "The Banished God",
    "Utter Darkness", "the Undead"
};

char *	const	month_name	[] =
{
    "Shadows", "Protection", "Gaia", "the Banshee",
    "the Wyrm", "Luna", "Lloth", "Spirit", "the Elders",
    "the Sun", "the Heat", "the Battle", "the Dark Shades", "the Shadows",
    "the Long Shadows", "the Ancient Darkness", "the Great Evil"
};

void do_time( CHAR_DATA *ch, char *argument )
{
    extern char str_boot_time[];
    char buf[MAX_STRING_LENGTH];
    char *suf;
    int day;

    day     = time_info.day + 1;

         if ( day > 4 && day <  20 ) suf = "th";
    else if ( day % 10 ==  1       ) suf = "st";
    else if ( day % 10 ==  2       ) suf = "nd";
    else if ( day % 10 ==  3       ) suf = "rd";
    else                             suf = "th";

    sprintf( buf,
	"It is %d o'clock %s, Day of %s, %d%s the Month of %s.\n\rADG started up at %s\rThe system time is %s\r",

	(time_info.hour % 12 == 0) ? 12 : time_info.hour % 12,
	time_info.hour >= 12 ? "pm" : "am",
	day_name[day % 7],
	day, suf,
	month_name[time_info.month],
	str_boot_time,
	(char *) ctime( &current_time )
	);

    send_to_char( buf, ch );
if (dxp_info.dxp == 1) {
         sprintf (buf, "{W%d %s {wleft of {BDouble Exp{w.{x\n\r",
                  dxp_info.dxpt >= 240 ? dxp_info.dxpt / 60 /
                  PULSE_PER_SECOND : dxp_info.dxpt / PULSE_PER_SECOND,
                  dxp_info.dxpt >= 240 ? "minute(s)" : "second(s)");
     
stc(buf,ch);}
if (chao_info.chs == 1) {
         sprintf (buf, "{W%d %s {wleft of #.C#.H#.A#.O#.S{w.{x\n\r",
                  chao_info.chst >= 240 ? chao_info.chst / 60 /
                  PULSE_PER_SECOND : chao_info.chst / PULSE_PER_SECOND,
                  chao_info.chst >= 240 ? "minute(s)" : "second(s)");
      
stc(buf,ch);}
if (dqp_info.dqp == 1) {
         sprintf (buf, "{W%d %s {wleft of {CDouble Quest Points{w.{x\n\r",
                  dqp_info.dqpt >= 240 ? dqp_info.dqpt / 60 /
                  PULSE_PER_SECOND : dqp_info.dqpt / PULSE_PER_SECOND,
                  dqp_info.dqpt >= 240 ? "minute(s)" : "second(s)");

stc(buf,ch);}
if (dtr_info.dtr == 1) {
         sprintf (buf, "{W%d %s {wleft of {YDouble Training{w.{x\n\r",
                  dtr_info.dtrt >= 240 ? dtr_info.dtrt / 60 /   
                  PULSE_PER_SECOND : dtr_info.dtrt / PULSE_PER_SECOND,
                  dtr_info.dtrt >= 240 ? "minute(s)" : "second(s)");
stc(buf,ch);}
if (IS_IMMORTAL(ch)){
sprintf(buf,"Next war in: %d\n\r",war_info.wart);
stc(buf,ch);}
    return;
}



void do_phase(CHAR_DATA *ch, char *argument)
{  
    if (!IS_OUTSIDE(ch))
    {
    send_to_char("You can't see the moon indoors.\n\r", ch);
    return;
    }
/*
 * Werewolves can sence the moonphases anytime
 */
    if (weather_info.moonlight == MOON_DOWN && IS_CLASS(ch, CLASS_WEREWOLF))
    {
    send_to_char("The moon is not up.\n\r", ch);
        return;
    }

    if (weather_info.moonphase == MOON_NEW)
    {
    send_to_char("The moon is new.\n\r", ch);
    return;
    }

    if (weather_info.moonphase == MOON_CRESCENT)
    {
    send_to_char("The moon is crescent.\n\r", ch);
    return;
    }

    if (weather_info.moonphase == MOON_HALF)
    {
    send_to_char("The moon is half tonight.\n\r", ch);
    return;
    }
 
    if (weather_info.moonphase == MOON_GIBBOUS)
    {
    send_to_char("The moon is in its gibbous phase.\n\r", ch);
    return;
    }

    if (weather_info.moonphase == MOON_FULL)
    {
    send_to_char("The moon is full tonight.\n\r", ch);
    return;
    }

    if (weather_info.moonphase == MOON_WANING)
    {
    send_to_char("The moon is waning.\n\r", ch);
    return;
    } 

    else
    {
    send_to_char("bug, please inform an implementor.\n\r", ch);
    return;
    }

}

    
void do_weather(CHAR_DATA *ch, char *argument)
{
    char buf[MAX_STRING_LENGTH];

    static char * const sky_look[4] =
    {
    "cloudless",
    "cloudy",
    "rainy",
    "lit by flashes of lightning"
    };

    if (!IS_OUTSIDE(ch))
    {
    send_to_char("You can't see the weather indoors.\n\r", ch);
    return;
    }

    if (IS_SET(world_affects, WORLD_ECLIPSE))
    send_to_char("The sun is eclipsed by the moon.\n\r", ch);

    if (IS_SET(world_affects, WORLD_FOG))
    send_to_char("A thick blanket of fog covers the ground.\n\r", ch);

    if (IS_SET(world_affects, WORLD_RAIN))
    {
    sprintf(buf, "The sky is pouring with a heavy rain and %s.\n\r",
    weather_info.change >= 0
    ? "a warm southerly breeze blows"
    : "a cold northern gust blows"
);
    send_to_char(buf, ch);
    return;
    }

    sprintf(buf, "The sky is %s and %s.\n\r",
    sky_look[weather_info.sky],
    weather_info.change >= 0
    ? "a warm southerly breeze blows"
    : "a cold northern gust blows"
);
    send_to_char(buf, ch);
    return;
}



void do_help( CHAR_DATA *ch, char *argument )
{
    char argall[MAX_INPUT_LENGTH];
    char argone[MAX_INPUT_LENGTH];
    char nohelp[MAX_STRING_LENGTH];
    HELP_DATA *pHelp;

    strcpy(nohelp, argument);

    if ( argument[0] == '\0' )
	argument = "summary";

    /*
     * Tricky argument handling so 'help a b' doesn't match a.
     */
    argall[0] = '\0';
    while ( argument[0] != '\0' )
    {
	argument = one_argument( argument, argone );
	if ( argall[0] != '\0' )
	    strcat( argall, " " );
	strcat( argall, argone );
    }

    for ( pHelp = help_first; pHelp != NULL; pHelp = pHelp->next )
    {
	if ( pHelp->level > get_trust( ch ) )
	    continue;

	if ( is_name( argall, pHelp->keyword ) )
	{
            if (!str_cmp(argall,"changes")) { do_changes(ch,""); return; }
	    if ( pHelp->level >= 0 && str_cmp( argall, "imotd" ) )
	    {
		send_to_char( pHelp->keyword, ch );
		send_to_char( "\n\r", ch );
 	    }
        
	    /*
	     * Strip leading '.' to allow initial blanks.
	     */
	    if ( pHelp->text[0] == '.' )
		send_to_char( pHelp->text+1, ch );
	    else
		send_to_char( pHelp->text  , ch );
	    return;
	}
    }

    send_to_char( "No help on that word.\n\r", ch );
    append_file( ch, HELP_FILE, nohelp );
    sprintf( log_buf, " {D[{WHelp{D]{w %s has requested help on %s.", ch->name, nohelp );
    log_string( log_buf );
    send_to_char( "The staff have been notified of it's absence.\n\r", ch );
    return;
}


int col_str_len(char *txt)
{
    int pos, len;

    len = 0;

    for (pos = 0; txt[pos] != '\0'; pos++)
    {
	if (txt[pos] == '{')
	    pos ++;
        else
            len++;

	continue;
    }

    return len;
}

void cent_to_char(char *txt, CHAR_DATA *ch)
{
    int len,pos;
    char buf[MAX_STRING_LENGTH];

    len = (80 - col_str_len(txt)) / 2;
    for (pos = 0; pos < len; pos++)
    {
	buf[pos] = ' ';
    }
    buf[pos]= '\0';
    send_to_char(buf, ch);
    send_to_char(txt, ch);
    send_to_char("\n\r",ch);
}

void divide_to_char(CHAR_DATA *ch)
{
send_to_char("#e-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-#n\r\n",ch);
}

void divide2_to_char(CHAR_DATA *ch)
{
send_to_char("-------------------------------------------------------------------------------\r\n",ch);
}

void divide3_to_char(CHAR_DATA *ch)
{
send_to_char("===============================================================================\r\n",ch);
}

void divide4_to_char(CHAR_DATA *ch)
{
send_to_char("-=[**]=-=[**]=-=[**]=-=[**]=-=[**]=-=[***]=-=[**]=-=[**]=-=[**]=-=[**]=-=[**]=-\r\n",ch);
}

void divide5_to_char(CHAR_DATA *ch)
{
cent_to_char("-=[***********]=-------------=[***********]=-",ch);
}

void divide6_to_char(CHAR_DATA *ch)
{
cent_to_char("-    -   -  - - -- ---====*====--- -- - -  -   -    -",ch);
}

void banner_to_char(char *txt, CHAR_DATA *ch)
{
char buf[MAX_STRING_LENGTH];
int loop,wdth,ln;
ln = strlen(txt);
if (ln > 16)
{
	sprintf(buf, "-=[**]=-=[**]=-=[**]=-=[                               ]=-=[**]=-=[**]=-=[**]=-"); 
	wdth = (31 - ln) / 2 + 20;
}
else
{
	sprintf(buf,
"-=[**]=-=[**]=-=[**]=-=[**]=-=[                ]=-=[**]=-=[**]=-=[**]=-=[**]=-");
	wdth = (16 - ln) / 2 + 32;
}
for (loop = 0; loop < ln; loop++)
    buf[loop + wdth + 22] = txt[loop];
cent_to_char(buf,ch);
}

void banner2_to_char(char *txt, CHAR_DATA *ch)
{
char buf[MAX_STRING_LENGTH];
int loop,wdth,ln;
ln = strlen(txt);
if (ln > 16)
{
	sprintf(buf, "    -   -  - - -- ---===                               ===--- -- - -  -   -\r\n"); 
	wdth = (31 - ln) / 2 + 24;
}
else
{
	sprintf(buf, "     -    -   -  - - -- ---====                ====--- -- - -  -   -    -\r\n");
	wdth = (16 - ln) / 2 + 32;
}
for (loop = 0; loop < ln; loop++)
    buf[loop + wdth + 6] = txt[loop];
send_to_char(buf,ch);
}

/*
 *  Bounty list
 *  - By Tijer
 */
void do_bountylist( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *gch;
    bool found = FALSE;

    one_argument( argument, arg );

    if (IS_NPC(ch)) return;

    send_to_char("People who have a bounty on them:\n\r", ch );
    send_to_char("#e[#c       Name#e       ] [#c Hit%#e ] [#c Mana% #e] [#c Move%#e ] [#c   Bounty #e  ]#n\n\r",ch);

    for ( gch = char_list; gch != NULL; gch = gch->next )
    {
        if ( IS_NPC(gch) ) continue; 
	if ( IS_IMMORTAL(gch) ) continue;
        if ( gch->pcdata->bounty == 0)
            continue;
     found = TRUE;
    
        sprintf( buf,
                "[%-18s] [ %-3ld%% ] [  %-3ld%% ] [  %-3ld%% ] [ %10d ]\n\r",
                capitalize( gch->name ),
                ((gch->hit * 100) / gch->max_hit),
                ((gch->mana * 100) / gch->max_mana),
                ((gch->move * 100) / gch->max_move),
                gch->pcdata->bounty);
                send_to_char( buf, ch );

    
    }
   if ( !found )
        send_to_char( "No-one currently connected has a bounty on them.\n\r", ch );
    return;
}
     
/*
 * New 'who' command originally by Alander of Rivers of Mud.
 * Major re-working to fit with AltReal.
 */
void do_who( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char buf2[MAX_STRING_LENGTH];
    char mort[MAX_STRING_LENGTH];
    char avat[MAX_STRING_LENGTH];
    char god[MAX_STRING_LENGTH];
    char a1[MSL];
    char a2[MSL];
    char a3[MSL];
    char a4[MSL];
    char a5[MSL];
    char a6[MSL];
    char a7[MSL];
    char a0[MSL];
    char immo[MAX_STRING_LENGTH];
    char kav[MAX_STRING_LENGTH];
    char kav2[MAX_STRING_LENGTH];
    char cls[MAX_STRING_LENGTH];
    char bty[MAX_STRING_LENGTH];
    char wizi[MAX_STRING_LENGTH];
    char dxp[MAX_STRING_LENGTH];
    char dqp[MAX_STRING_LENGTH];
    char dtr[MAX_STRING_LENGTH];
    char chs[MAX_STRING_LENGTH];
    char openb[5];
    char namething[MSL];
    char closeb[5];
    char const *status_colour;
    DESCRIPTOR_DATA *d;
    int iLevelLower;
    int iLevelUpper;
    int nNumber;
    int immMatch, nMatch;
    int mor, ava, imm;
    bool fClassRestrict;
    bool fImmortalOnly;
    bool canDecap;
    bool canKillyou;

    if (IS_NPC(ch)) return;

    /*
     * Set default arguments.
     */
    status_colour  = "#r";
    iLevelLower    = 0;
    iLevelUpper    = MAX_LEVEL +1;
    fClassRestrict = FALSE;
    fImmortalOnly  = FALSE;

    /*
     * Parse arguments.
     */
    nNumber = 0;
    for ( ;; )
    {
	char arg[MAX_STRING_LENGTH];

	argument = one_argument( argument, arg );
	if ( arg[0] == '\0' )
	    break;

	if ( is_number( arg ) )
	{
	    send_to_char("Enter 'Avatar' for level 3's, or 'God' for level 4's and 5's.\n\r",ch);
	    return;
	}
	else
	{
	    /*
	     * Look for classes to turn on.
	     */
	    arg[3]    = '\0';
	    if (   !str_cmp( arg, "imm" ) || !str_cmp( arg, "immortal" )
		|| !str_cmp( arg, "ava" ) || !str_cmp( arg, "avatar"   ) )
	    {
		fClassRestrict = TRUE;
	    }
	    else if ( !str_cmp( arg, "god" ) || !str_cmp( arg, "imp" ) )
	    {
		fImmortalOnly = TRUE;
	    }
	    else
	    {
		send_to_char("Enter 'Avatar' for level 3's, or 'God' for level 4's and 5's.\n\r",ch);
		return;
	    }
	}
    }

    /*
     * Now show matching chars.
     */
    immMatch = 0;
    nMatch = 0;
    buf[0] = '\0';
    mort[0] = '\0';
    avat[0] = '\0';
    immo[0] = '\0';
    a1[0] = '\0';
    a2[0] = '\0';
    a3[0] = '\0';
    a4[0] = '\0';
    a5[0] = '\0';
    a6[0] = '\0';
    a7[0] = '\0';
    a0[0] = '\0';

    mor = ava = imm = 0;
    for ( d = descriptor_list; d != NULL; d = d->next )
    {
	CHAR_DATA *wch;
	char const *class;
	char const *lstatus;

	/*
	 * Check for match against restrictions.
	 * Don't use trust as that exposes trusted mortals.
	 */
	if ( d->connected != CON_PLAYING || (!can_see( ch, d->character ) && (!IS_SET(ch->act, PLR_WATCHER)) )  )
	    continue;

	wch   = ( d->original != NULL ) ? d->original : d->character;
	if ( wch->level < iLevelLower
	||   wch->level > iLevelUpper
	|| ( fImmortalOnly  && wch->level <  LEVEL_IMMORTAL )
	|| ( fClassRestrict && wch->level != LEVEL_HERO     ) )
	    continue;

    canDecap = fair_fight(ch, wch);
    canKillyou = fair_fight(wch, ch);

//    if(canDecap) status_colour = "#b";
//	else if (canKillyou) status_colour = "#o";
//	else if (canDecap) status_colour = "#y";
//	else status_colour = "{c";

    if (ch->race == 0 && wch->race >= 1) status_colour = "#b";
    else if (wch->race == 0 && ch->race >= 1) status_colour = "{c";
    else if (ch->race >= 1 && wch->race > ch->race + 10) status_colour = "#b";
    else if (wch->race >= 1 && ch->race < wch->race + 10) status_colour = "#b";
    else if ( (( get_age(wch) - 17) * 2 ) < 5 ) status_colour = "{c";
    else if (ch->race >= 1 && wch->race == 0) status_colour = "{c";
    else status_colour = "{c";

    if (!str_cmp(wch->name,ch->name)) status_colour = "{Y";

       if (wch->level > 6)
          immMatch++;
        else
          nMatch++;
	/*
	 * Figure out what to print for class.
	 */
	 class   = "";
//         lstatus = "";

	switch( wch->lstatus )
        {
          default:    lstatus = "#c 0#n"; break;
          case L_CIT: lstatus = "{Y 1#n"; break;
          case L_LEG: lstatus = "{y 2#n"; break;
	  case L_MYT: lstatus = "{C 3#n"; break;
          case L_TIT: lstatus = "{G 4#n"; break;
          case L_ORA: lstatus = "{m 5#n"; break;
          case L_IMM: lstatus = "{B 6#n"; break;
	  case L_DEM: lstatus = "{b 7#n"; break;
          case L_DEI: lstatus = "{M 8#n"; break;
          case L_ALM: lstatus = "{W 9#n"; break;
        }

    if ( ( IS_HEAD(wch,LOST_HEAD) || IS_EXTRA(wch,EXTRA_OSWITCH) ) && 
    wch->pcdata->chobj != NULL )
    {
        switch ( wch->pcdata->chobj->pIndexData->vnum )
        {
            default:         class = "#CAn Object#n ";    break;
            case 12:         class = "#CA Head#n    ";    break;
            case 30005:      class = "#CA Brain#n   ";    break;
        }
    }
    else if(IS_SET(wch->extra, TIED_UP))
                             class = " #CTied Up#n  ";

    else if(IS_SET(wch->flag2, EXTRA_AFK))
                             class = " #w->#rAFK#w<-#n  ";

    else if(IS_SET(wch->newbits, TEST_CHAR))
                             class = "#gTest Char#n ";

    else if ( wch->desc != NULL && wch->desc->connected != CON_PLAYING )
    {
        if( wch->desc->connected == CON_NOTE_TO      ||
            wch->desc->connected == CON_NOTE_SUBJECT ||
            wch->desc->connected == CON_NOTE_EXPIRE  ||
            wch->desc->connected == CON_NOTE_TEXT    ||
            wch->desc->connected == CON_NOTE_FINISH)
                             class = "#eNoteEditor#n";
        else if( wch->desc->connected > CON_PLAYING )
                             class = "#wConnecting#n";
        else if( wch->desc->connected == CON_AEDITOR ||
                 wch->desc->connected == CON_REDITOR ||
                 wch->desc->connected == CON_MEDITOR ||
                 wch->desc->connected == CON_OEDITOR)
                             class = "#g [<OLC>]  #n";
    }
    else switch ( wch->level )
    {

    default: break;
    case MAX_LEVEL -  0:     class= "#c   Pimp   #n"; break; /*Implamentor*/
    case MAX_LEVEL -  1:     class= "#c Immortal #n"; break; /*HighJudge*/
    case MAX_LEVEL -  2:     class= "#c Immortal #n"; break; /*Judge*/
    case MAX_LEVEL -  3:     class= "#c Immortal #n"; break; /*Enforcer*/
    case MAX_LEVEL -  4:     class= "#c Immortal #n"; break; /*Questmakers*/
    case MAX_LEVEL -  5:     class= "#c Immortal #n"; break; /*Builder*/
    case MAX_LEVEL -  6:
    case MAX_LEVEL -  7:
    case MAX_LEVEL -  8:
    case MAX_LEVEL -  9: 
    if (wch->race <= 0 )     class = "Avatar    ";
    else if (wch->race < 10) class = "Page      ";
    else if (wch->race < 20) class = "Knight    ";
    else if (wch->race < 30) class = "Count     ";
    else if (wch->race < 40) class = "VisCount  ";
    else if (wch->race < 50) class = "Lord      ";
    else if (wch->race < 60) class = "Baron     ";
    else if (wch->race < 70) class = "Duke      ";
    else                     class = "Prince    ";
    break;
    case MAX_LEVEL - 10:     class = "#yMortal    #n"; break;
    case MAX_LEVEL - 11:     class = "#yNewbie    #n"; break;
    case MAX_LEVEL - 12:     class = "#yNewbie    #n"; break;
    }
    /*
     * Format it up.
     */
    if ( IS_ABOMINATION(wch) )
    { strcpy( openb, "#R<#o(#n" ); strcpy (closeb, "#o)#R>#n" ); }
    else if ( IS_FAE(wch))
    { strcpy( openb, "#P--" ); strcpy( closeb, "--#n" ); }
    else if (IS_DEMON(wch) )
    { strcpy( openb, "#r[[" ); strcpy( closeb, "]]#n" ); }
    else if ( IS_HIGHLANDER(wch) || IS_DEATHKNIGHT(wch))
    { strcpy( openb, "#c-=" ); strcpy( closeb, "=-#n" ); }
    else if ( IS_HUNTER(wch))
    { strcpy( openb, "#R*#o*#w" ); strcpy( closeb, "#o*#R*#n" ); }
    else if ( IS_MAGE(wch))
    { strcpy( openb, "#b{{" ); strcpy( closeb, "#b}#n" ); }
    else if ( IS_SHAKRIN(wch))
    { strcpy( openb, "{r-{y-{g<" ); strcpy( closeb, "{g>{y-{r-{x" ); }
    else if ( IS_VAMPIRE(wch) )
    { strcpy( openb, "#R<<" ); strcpy( closeb, ">>#n" ); }
    else if ( IS_WEREWOLF(wch))
    { strcpy( openb, "#o((" ); strcpy( closeb, "))#n" ); }
    else if ( IS_WRAITH(wch) )
    { strcpy( openb, "#e|["); strcpy( closeb, "]|#n" ); }
    else if ( IS_NINJA(wch))
    { strcpy( openb, "{m**" ); strcpy( closeb, "**{x" ); }
    else if ( IS_MONK(wch))
    { strcpy( openb, "{c<{B:" ); strcpy( closeb, "{B:{c>{x" ); }

    else if ( IS_CLASS(wch, CLASS_DRAGON) )
    { strcpy( openb, "{M(=" ); strcpy( closeb, "=){x" ); }
/*    else if ( IS_CLASS(wch, CLASS_ELADRIN) )
    { strcpy( openb, "{G==)"); strcpy( closeb, "(=={x" ); }
*/
    else if ( IS_DROW(wch))
    { strcpy( openb, "{b.o0 "); strcpy(closeb, " 0o.{x" ); }
    else
    { strcpy( openb, "#w[" ); strcpy( closeb, "]#n" ); }

    if ( ch->class > 0 || IS_IMMORTAL( ch ))
    {
        if ( IS_DEMON(wch) && wch->pcdata->stats[UNI_GEN] == 1 )
            sprintf( kav, "%sFallen Master%s", openb, closeb);
	else if (IS_FAE(wch) && wch->pcdata->stats[UNI_GEN] == 1 )
	    sprintf( kav, "%sMaster Fae%s", openb, closeb );
	else if (IS_WEREWOLF(wch) && wch->pcdata->stats[UNI_GEN] == 1 )
	    sprintf( kav, "%sMaster Werewolf%s", openb, closeb );
	else if (IS_HIGHLANDER(wch) && wch->pcdata->stats[UNI_GEN] == 1 )
	    sprintf( kav, "%sKensai Master%s", openb, closeb );
        else if ( IS_HUNTER(wch) && wch->pcdata->stats[UNI_GEN] == 1 )
            sprintf( kav, "%sInquisitor General%s", openb, closeb);
	else if ( IS_VAMPIRE(wch) && wch->pcdata->stats[UNI_GEN] == 1 )
	    sprintf( kav, "%sMaster Vampire%s", openb, closeb );
	else if ( IS_MAGE(wch) && wch->pcdata->stats[UNI_GEN] == 1 )
	    sprintf( kav, "%sWarlock Master%s", openb, closeb );
	else if ( IS_WRAITH(wch) && wch->pcdata->stats[UNI_GEN] == 1 )
	    sprintf( kav, "%sMaster Wraith%s", openb, closeb );
	else if ( IS_DEATHKNIGHT(wch) )
	    sprintf( kav, "%s#rDeath Knight#c%s", openb, closeb);
	else if (IS_NINJA(wch) && wch->pcdata->stats[UNI_GEN] == 1 )
	    sprintf( kav, "%sNinja Leader%s", openb, closeb );
	else if (IS_MONK(wch) && wch->pcdata->stats[UNI_GEN] == 1 )
	    sprintf( kav, "%sGod's Abbot%s", openb, closeb );
	else if (IS_DROW(wch) && wch->pcdata->stats[UNI_GEN] == 1 )
            sprintf( kav, "%sLloth's Avatar%s", openb, closeb );

        else if ( IS_CLASS(wch, CLASS_DEMON) && IS_SET(wch->special, SPC_DEMON_LORD) )
	    	sprintf( kav, "%sLord of %s%s", openb, wch->clan, closeb );
	    else if ( IS_SET(wch->special, SPC_CHAMPION) )
	      {
		if (IS_SET(wch->special, SPC_PRINCE) && wch->sex == SEX_FEMALE)
		    sprintf( kav, "%sPrincess of %s%s", openb, wch->clan,closeb );
		else if (IS_SET(wch->special, SPC_PRINCE))
		    sprintf( kav, "%sPrince of %s%s", openb,wch->clan,closeb );
		else if (IS_SET(wch->special, SPC_SIRE))
		    sprintf( kav, "%sSire of %s%s", openb,wch->clan,closeb );
                else if (strlen(wch->clan) < 2)
	            sprintf( kav, "%sDemon%s", openb,closeb); 
                else
                    sprintf( kav, "%sChampion of %s%s", openb,wch->clan,closeb );
	      }
            else if(IS_DEMON(wch))
                sprintf( kav, "%sDemon%s", openb,closeb);

	    else if ( IS_VAMPIRE(wch) && wch->clan != NULL )
	    {
		bool get_age = FALSE;
		char clanname[20];

		if (IS_SET(wch->special, SPC_INCONNU))
		    get_age = TRUE;
	    	else if (IS_SET(wch->special, SPC_ANARCH))
		    get_age = TRUE;
	    	else if (strlen(wch->clan) < 2)
		    get_age = TRUE;
	    	else if (wch->pcdata->stats[UNI_GEN] == 2)
	    	    sprintf( kav, "%s%s Primogen%s", openb, wch->clan, closeb );
	    	else if (IS_SET(wch->special, SPC_PRINCE) && wch->sex == SEX_FEMALE)
	    	    sprintf( kav, "%s%s Princess%s", openb, wch->clan, closeb );
	    	else if (IS_SET(wch->special, SPC_PRINCE))
	    	    sprintf( kav, "%s%s Prince%s", openb, wch->clan, closeb );
	    	else get_age = TRUE;
		if (get_age)
		{
		    if (IS_SET(wch->special, SPC_INCONNU))
			strcpy( clanname, "Inconnu" );
		    else if (IS_SET(wch->special, SPC_ANARCH))
			strcpy( clanname, "Anarch" );
		    else if (strlen(wch->clan) < 2)
			strcpy( clanname, "Caitiff" );
		    else
			strcpy( clanname, wch->clan );
		    if (wch->pcdata->stats[UNI_GEN] == 2)
			sprintf( kav, "%s%s Antediluvian%s", openb, clanname, closeb );
		    else switch ( wch->pcdata->rank )
		    {
		    default:
			sprintf( kav, "%s%s Childe%s", openb, clanname, closeb );
			break;
		    case AGE_NEONATE:
			sprintf( kav, "%s%s Neonate%s", openb, clanname, closeb );
			break;
		    case AGE_ANCILLA:
			sprintf( kav, "%s%s Ancilla%s", openb, clanname, closeb );
			break;
		    case AGE_ELDER:
			sprintf( kav, "%s%s Elder%s", openb, clanname, closeb );
			break;
		    case AGE_METHUSELAH:
			sprintf( kav, "%s%s Methuselah%s", openb, clanname, closeb );
			break;
		    }
		}
	    }

	    else if ( IS_WEREWOLF(wch)
		&& wch->clan != NULL && strlen(wch->clan) > 1 )
	    {
	    	if (wch->pcdata->stats[UNI_GEN] == 2)
	    	    sprintf( kav, "%s%s Chief%s", openb, wch->clan, closeb );
	    	else if (IS_SET(wch->special, SPC_PRINCE))
	    	    sprintf( kav, "%s%s Shaman%s", openb, wch->clan, closeb );
	    	else
	    	    sprintf( kav, "%s%s%s", openb, wch->clan, closeb );
	    }

	    else if ( IS_WEREWOLF(wch)
		&& wch->clan != NULL && strlen(wch->clan) < 1 )
	    	sprintf( kav, "%sRonin%s", openb, closeb );

	    else if ( IS_SHAKRIN(wch)
		&& wch->clan != NULL && strlen(wch->clan) < 1 )
		sprintf( kav, "%s#uShakrin#n%s", openb, closeb );

else if ( IS_CLASS(wch, CLASS_DRAGON) && wch->pcdata->dragonage == 0)
sprintf( kav,"{C+{m({MDragon{m){C+{x");
else if ( IS_CLASS(wch, CLASS_DRAGON) && wch->pcdata->dragonage > 0)
sprintf( kav,"{C+{m({M%s Dragon{m){C+{x",dage_table[wch->pcdata->dragonage-1].name);
	    else if ( IS_DROW(wch)
	    && wch->clan != NULL && strlen(wch->clan) > 1 )
          {
		if (wch->pcdata->stats[UNI_GEN] == 2)
			sprintf(kav,"%s%s Matron%s", openb, wch->clan, closeb );
		else if (IS_SET(wch->special, SPC_DROW_WAR))
			sprintf(kav, "%s%s Warrior%s", openb, wch->clan, closeb );
		else if (IS_SET(wch->special, SPC_DROW_CLE))
			sprintf(kav, "%s%s Cleric%s", openb, wch->clan, closeb );
		else if (IS_SET(wch->special, SPC_DROW_MAG))
			sprintf(kav, "%s%s Mage%s", openb, wch->clan, closeb );
                else sprintf(kav, "%s%s Drow%s", openb, wch->clan, closeb);
          }
	    else if ( IS_DROW(wch)
          && wch->clan != NULL && strlen(wch->clan) < 1 )
              sprintf( kav, "%sDrow Outcast%s", openb, closeb );
/*
	    else if (IS_CLASS(wch, CLASS_MOOGLE))
	    {
		sprintf(kav, "%sMoogle%s", openb, closeb);
	    }

	    else if ( IS_CLASS(wch, CLASS_PALADIN) )
	    {
		sprintf( kav, "%sPaladin%s", openb, closeb);
	    }
*/
	    else if ( IS_MONK(wch)
		&& wch->clan != NULL && strlen(wch->clan) > 1 )
	    {
	    	if (wch->pcdata->stats[UNI_GEN] == 2)
	    	    sprintf( kav, "%s{wAbbot of %s%s", openb, wch->clan, closeb );
	    	else if (IS_SET(wch->special, SPC_PRINCE))
	    	    sprintf( kav, "%s{wPrince of %s%s", openb, wch->clan, closeb );
	    	else if (IS_SET(wch->special, SPC_SIRE))
	    	    sprintf( kav, "%s%s {wSifu%s", openb, wch->clan, closeb );
	    	else
	    	    sprintf( kav, "%s%s {wMonk%s", openb, wch->clan, closeb );
	    }
	    else if ( IS_MONK(wch)
		&& wch->clan != NULL && strlen(wch->clan) < 1 )
	    	sprintf( kav, "%s{wMonk Outcast%s", openb, closeb );

	    else if ( IS_NINJA(wch)
	              && wch->clan != NULL && strlen(wch->clan) > 1 )
	    {
	    	if (wch->pcdata->stats[UNI_GEN] == 2)
	    	    sprintf( kav, "%sJonin of %s%s", openb, wch->clan, closeb );
	    	else if (IS_SET(wch->special, SPC_PRINCE))
	    	    sprintf( kav, "%sChunin of %s%s", openb, wch->clan, closeb );    	
	    	else
	    	    sprintf( kav, "%sGenin of %s%s", openb, wch->clan, closeb );
	    }
	    else if ( IS_NINJA(wch)
	              && wch->clan != NULL && strlen(wch->clan) < 1)
	    {
	      if (wch->pcdata->stats[UNI_GEN] == 2)
	        sprintf( kav, "%sNinja Leader%s", openb, closeb );
	      else if (IS_SET(wch->special, SPC_PRINCE))
	        sprintf( kav, "%sChunin Ninja%s", openb, closeb );
	      else if (IS_SET(wch->special, SPC_ROGUE))
			sprintf( kav, "%sRogue Ninja%s", openb, closeb );
	      else
	       	sprintf( kav, "%sNinja Outcast%s", openb, closeb );
	    }


/*
	else if ( IS_CLASS(wch, CLASS_MAGE) )
        {
                char *mage_col;
                switch ( wch->pcdata->powersm[MPOWER_RUNE0] )
                {
                    default:
                    case PURPLE_MAGIC:  mage_col = "#P";   break;
                    case RED_MAGIC:     mage_col = "#r";  break;
                    case BLUE_MAGIC:    mage_col = "#b"; break; 
                    case GREEN_MAGIC:   mage_col = "#g"; break;
                    case YELLOW_MAGIC:  mage_col = "#y";break;
                }
		if(strlen(wch->clan            ) > 2)
		{
		  if(IS_SET(wch->malbits,CLAN_LEADER))
			sprintf(kav,"%s%sMaster#b of %s%s#n",
			  openb,mage_col,wch->clan,closeb);
		  else if(IS_SET(wch->malbits,CLAN_SIRE))
			sprintf(kav,"%s%sHigh Adept#b of %s%s#n",
			  openb,mage_col,wch->clan,closeb);
		  else sprintf(kav,"%s%sAdept#b of %s%s#n",
			  openb,mage_col,wch->clan,closeb);
		}
		else sprintf(kav,"%s%sHollow Ones%s#n",
			openb,mage_col,closeb);	
}
*/
	else if ( IS_MAGE(wch) )
        {
                char *mage_col;
                switch ( wch->pcdata->powersm[MPOWER_RUNE0] )
                {
                    default:
                    case PURPLE_MAGIC:  mage_col = "#PAuram#n";   break;
                    case RED_MAGIC:     mage_col = "#rIgnem#n";  break;
                    case BLUE_MAGIC:    mage_col = "#bAquam#n"; break; 
                    case GREEN_MAGIC:   mage_col = "#gTerram#n"; break;
                    case YELLOW_MAGIC:  mage_col = "#yMentem#n";break;
                }
            
                switch ( wch->level )
                {
                   default:
            case LEVEL_APPRENTICE:
            sprintf( kav, "%sAcolyte of %s%s", openb, mage_col, closeb); 
            break;
            case LEVEL_MAGE:
            sprintf( kav, "%sAdept of %s%s", openb, mage_col, closeb ); 
            break;
            case LEVEL_ARCHMAGE:
            sprintf( kav, "%sArchmage of %s%s", openb, mage_col, closeb ); 
            break;
               }
            }
          
            else if ( IS_WRAITH(wch) )
            {
                sprintf( kav, "%sWraith%s", openb, closeb);
            }

	    else if ( IS_HIGHLANDER(wch) )
	    {
		sprintf( kav, "%sHighlander%s", openb, closeb);
	    }

	    else if ( IS_ABOMINATION(wch) )
	    {
		sprintf( kav, "%sAbomination%s", openb, closeb);
	    }

          else if ( IS_FAE(wch) )
          {
              sprintf( kav, "%sFae%s", openb, closeb);
          }

          else if ( IS_HUNTER(wch) )
          {
              sprintf( kav, "%sImbued%s", openb, closeb);
          }
    else
        strcpy( kav, "#CHUMAN#n" );
	}
	else
	    strcpy( kav, "" );

        if (wch->class_2 != 0) {
            if (wch->class == CLASS_DRAGON) sprintf(kav,"{C+{m({MDragon{m){C+{x");
            else if (wch->class == CLASS_DEMON) sprintf(kav,"{r[{RDemon{r]{x");
            else if (wch->class == CLASS_WEREWOLF) sprintf(kav,"{y({YWolf{y){x"); 
            else if (wch->class == CLASS_HIGHLANDER) sprintf(kav,"{C-=Kensai=-{x"); 
            else if (wch->class == CLASS_VAMPIRE) sprintf(kav,"{r<Vamp>{x"); 
            else if (wch->class == CLASS_MAGE) sprintf(kav,"{r{{{R{{{wMage{r}{R}{x"); 
            else if (wch->class == CLASS_DROW) sprintf(kav,"{m.{Mo{D0{WDrow{D0{Mo{m.{x"); 
            else if (wch->class == CLASS_MONK) sprintf(kav,"{c<{B:{wMonk{B:{c>{x"); 
            else if (wch->class == CLASS_NINJA) sprintf(kav,"{m*Ninja*{x"); 
            else sprintf(kav2,"{cHUMAN{x");
            if (wch->class_2 == CLASS_DEMON) sprintf(kav2,"{W/ {r[{RDemon{r]{x");
            else if (wch->class_2 == CLASS_DRAGON) sprintf(kav2,"{W/ {C+{m({MDragon{m){C+{x");
            else if (wch->class_2 == CLASS_DEMON) sprintf(kav2,"{W/ {r[{RDemon{r]{x");        
            else if (wch->class_2 == CLASS_WEREWOLF) sprintf(kav2,"{W/ {y({YWolf{y){x"); 
            else if (wch->class_2 == CLASS_HIGHLANDER) sprintf(kav2,"{W/ {C-=Kensai=-{x");  
            else if (wch->class_2 == CLASS_VAMPIRE) sprintf(kav2,"{W/ {r<Vamp>{x");     
            else if (wch->class_2 == CLASS_MAGE) sprintf(kav2,"{W/ {r{{{R{{{wMage{r}{R}{x");
            else if (wch->class_2 == CLASS_DROW) sprintf(kav2,"{W/ {m.{Mo{D0{WDrow{D0{Mo{m.{x");
            else if (wch->class_2 == CLASS_MONK) sprintf(kav2,"{W/ {c<{B:{wMonk{B:{c>{x");
            else if (wch->class_2 == CLASS_NINJA) sprintf(kav2,"{W/ {m*Ninja*{x");    
            else
            sprintf(kav2,"{cHUMAN{x");
        } else { sprintf(kav2,"{x"); }

        sprintf(cls,"%s %s",kav,kav2);

        if (wch->class_2 > 0 && strlen(wch->pcdata->titlec) > 3)  
        { sprintf(cls,"%s",wch->pcdata->titlec); }

        if (wch->pcdata->bounty > 0)
           sprintf( bty, "%d", wch->pcdata->bounty);
        else
           strcpy( bty, " ");

        if (wch->pcdata->wizinvis > 6)
           sprintf( wizi, "(%d)", wch->pcdata->wizinvis);
        else
           strcpy( wizi, "");

      sprintf(namething,"{W%s{w%s",wch->name,wch->pcdata->title);
	

      if ( wch->level > 6 )
      {
	sprintf( immo + strlen(immo), " {m[{c%s{m] {D%s {W%s {W%s{w%s{x\n\r",
	    class,
          wizi,
          wch->prefix,
	    wch->name,
	    wch->pcdata->title);
	imm++;
      }
      else if ( wch->level > 2 && wch->level < 7 )
      {
        sprintf( a0 + strlen(a0), " {m[{c%s%s{m][{D%s{m]{w %s %s {y%s{x\n\r",
            status_colour,
	    class,
            lstatus,
	    namething,
            cls,
	    king_table[wch->pcdata->kingdom].who_name );
  	    ava++;
      }
      else
      {
        sprintf( mort + strlen(mort), " {m[{c%s{m]{W %s{x\n\r",
            class,
	    namething );
	mor++;
      }
    }

    stc("\n\r",ch);
stc("{m ... {CAnother {DDarn {cGodWars{x\n\r",ch);
stc("{D -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-{x\n\r",ch);
    if ( imm > 0 )
    {
      stc(immo,ch);
stc("{D -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-{x\n\r",ch);
    }

    if ( ava > 0 )
    {
      stc(a0,ch);
stc("{D -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-{x\n\r",ch);
    }

  if ( mor > 0 )
    {
     stc(mort,ch); 
stc("{D -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-{x\n\r",ch);
    }
if (immMatch == 1)
        sprintf( god, "1");
    else if (immMatch > 1)
    	sprintf( god, "%d",immMatch);
    if (nMatch == 1)
        sprintf( buf2, "1");
    else
       	sprintf( buf2, "%d",nMatch); 
if (immMatch == 0) sprintf( god,"0");
if (nMatch == 0) sprintf(buf2,"0");
if (dxp_info.dxp == 1 ) { sprintf(dxp,"x"); }
else if (dxp_info.dxp == 0 ) { sprintf(dxp," "); }
if (dqp_info.dqp == 1 ) { sprintf(dqp,"x"); }
else if (dqp_info.dqp == 0 ) { sprintf(dqp," "); }
if (dtr_info.dtr == 1 ) { sprintf(dtr,"x"); }
else if (dtr_info.dtr == 0 ) { sprintf(dtr," "); }
if (chao_info.chs == 1 ) { sprintf(chs,"x"); }
else if (chao_info.chs == 0 ) { sprintf(chs," "); }
sprintf(buf,"{w Double XP {D[{m%s{D]{w            Double QP {D[{m%s{D]{w            Double Train {D[{m%s{D]{w            Chaos {D[{m%s{D]{x\n\r",dxp,dqp,dtr,chs);
stc(buf,ch);
sprintf(buf,"{c                           Players{D: {m%s{c              Immortals{D:{m %s{x\n\r",buf2,god);
stc(buf,ch);
stc("{D -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-{x\n\r",ch);
/*
    if (nMatch > 0)
       cent_to_char( buf2, ch );
    if (imm > 0 )
       cent_to_char( god,  ch );*/
  return;
}



void do_inventory( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    OBJ_DATA *portal;
    OBJ_DATA *portal_next;
    ROOM_INDEX_DATA *pRoomIndex;
    ROOM_INDEX_DATA *location;
    bool found;

    if (!IS_NPC(ch) && IS_HEAD(ch,LOST_HEAD))
        {send_to_char( "You are not a container.\n\r", ch ); return;}
    else if (!IS_NPC(ch) && IS_EXTRA(ch,EXTRA_OSWITCH))
    {
        if ( !IS_NPC(ch) && (obj = ch->pcdata->chobj) == NULL)
	    {send_to_char( "You are not a container.\n\r", ch ); return;}
	switch ( obj->item_type )
	{
	default:
	    send_to_char( "You are not a container.\n\r", ch );
	    break;

	case ITEM_PORTAL:
	    pRoomIndex = get_room_index(obj->value[0]);
	    location = ch->in_room;
	    if ( pRoomIndex == NULL )
	    {
		send_to_char( "You don't seem to lead anywhere.\n\r", ch );
		return;
	    }
	    char_from_room(ch);
	    char_to_room(ch,pRoomIndex);

	    found = FALSE;
	    for ( portal = ch->in_room->contents; portal != NULL; portal = portal_next )
	    {
		portal_next = portal->next_content;
		if ( ( obj->value[0] == portal->value[3]  )
		    && (obj->value[3] == portal->value[0]) )
		{
		    found = TRUE;
		    if (IS_AFFECTED(ch, AFF_SHADOWPLANE) &&
			!IS_SET(portal->extra_flags, ITEM_SHADOWPLANE) )
		    {
			REMOVE_BIT(ch->affected_by, AFF_SHADOWPLANE);
	    		do_look(ch,"auto");
			SET_BIT(ch->affected_by, AFF_SHADOWPLANE);
			break;
		    }
		    else if (!IS_AFFECTED(ch, AFF_SHADOWPLANE) &&
			IS_SET(portal->extra_flags, ITEM_SHADOWPLANE) )
		    {
			SET_BIT(ch->affected_by, AFF_SHADOWPLANE);
	    		do_look(ch,"auto");
			REMOVE_BIT(ch->affected_by, AFF_SHADOWPLANE);
			break;
		    }
		    else
		    {
	    		do_look(ch,"auto");
			break;
		    }
	    	}
	    }
	    char_from_room(ch);
	    char_to_room(ch,location);
	    break;

	case ITEM_DRINK_CON:
	    if ( obj->value[1] <= 0 )
	    {
		send_to_char( "You are empty.\n\r", ch );
		break;
	    }
	    if (obj->value[1] < obj->value[0] / 5)
	    	sprintf( buf, "There is a little %s liquid left in you.\n\r",liq_table[obj->value[2]].liq_color);
	    else if (obj->value[1] < obj->value[0] / 4)
	    	sprintf( buf, "You contain a small about of %s liquid.\n\r",liq_table[obj->value[2]].liq_color);
	    else if (obj->value[1] < obj->value[0] / 3)
	    	sprintf( buf, "You're about a third full of %s liquid.\n\r",liq_table[obj->value[2]].liq_color);
	    else if (obj->value[1] < obj->value[0] / 2)
	    	sprintf( buf, "You're about half full of %s liquid.\n\r",liq_table[obj->value[2]].liq_color);
	    else if (obj->value[1] < obj->value[0])
	    	sprintf( buf, "You are almost full of %s liquid.\n\r",liq_table[obj->value[2]].liq_color);
	    else if (obj->value[1] == obj->value[0])
	    	sprintf( buf, "You're completely full of %s liquid.\n\r",liq_table[obj->value[2]].liq_color);
	    else
	    	sprintf( buf, "Somehow you are MORE than full of %s liquid.\n\r",liq_table[obj->value[2]].liq_color);
	    send_to_char( buf, ch );
	    break;

	case ITEM_CONTAINER:
	case ITEM_CORPSE_NPC:
	case ITEM_CORPSE_PC:
	    act( "$p contain:", ch, obj, NULL, TO_CHAR );
	    show_list_to_char( obj->contains, ch, TRUE, TRUE );
	    break;
	}
	return;
    }
/*
    if(!IS_NPC(ch) && IS_CLASS(ch, CLASS_WEREWOLF)
	 && ch->pcdata->stats[UNI_RAGE] > 100)
    {
    send_to_char( "Your inventory is covered in fur.\n\r", ch);
    return;
    }*/

    send_to_char( "You are carrying:\n\r", ch );
    show_list_to_char( ch->carrying, ch, TRUE, TRUE );
    return;
}



void do_equipment( CHAR_DATA *ch, char *argument )
{
    OBJ_DATA *obj;
    int iWear;
    bool found;

    send_to_char( "You are using:\n\r", ch );
    found = FALSE;
    for ( iWear = 0; iWear < MAX_WEAR; iWear++ )
    {
	if ( ( obj = get_eq_char( ch, iWear ) ) == NULL )
	    continue;

	send_to_char( where_name[iWear], ch );
	if ( can_see_obj( ch, obj ) )
	{
	    send_to_char( format_obj_to_char( obj, ch, TRUE ), ch );
	    send_to_char( "\n\r", ch );
	}
	else
	{
	    send_to_char( "something.\n\r", ch );
	}
	found = TRUE;
    }

    if ( !found )
	send_to_char( "Nothing.\n\r", ch );

    return;
}



void do_compare( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    OBJ_DATA *obj1;
    OBJ_DATA *obj2;
    int value1;
    int value2;
    char *msg;

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );
    if ( arg1[0] == '\0' )
    {
	send_to_char( "Compare what to what?\n\r", ch );
	return;
    }

    if ( ( obj1 = get_obj_carry( ch, arg1 ) ) == NULL )
    {
	send_to_char( "You do not have that item.\n\r", ch );
	return;
    }

    if ( arg2[0] == '\0' )
    {
	for ( obj2 = ch->carrying; obj2 != NULL; obj2 = obj2->next_content )
	{
	    if ( obj2->wear_loc != WEAR_NONE
	    &&   can_see_obj( ch, obj2 )
	    &&   obj1->item_type == obj2->item_type
	    && ( obj1->wear_flags & obj2->wear_flags & ~ITEM_TAKE) != 0 )
		break;
	}

	if ( obj2 == NULL )
	{
	    send_to_char( "You aren't wearing anything comparable.\n\r", ch );
	    return;
	}
    }
    else
    {
	if ( ( obj2 = get_obj_carry( ch, arg2 ) ) == NULL )
	{
	    send_to_char( "You do not have that item.\n\r", ch );
	    return;
	}
    }
	    
    msg		= NULL;
    value1	= 0;
    value2	= 0;

    if ( obj1 == obj2 )
    {
	msg = "You compare $p to itself.  It looks about the same.";
    }
    else if ( obj1->item_type != obj2->item_type )
    {
	msg = "You can't compare $p and $P.";
    }
    else
    {
	switch ( obj1->item_type )
	{
	default:
	    msg = "You can't compare $p and $P.";
	    break;

	case ITEM_ARMOR:
	    value1 = obj1->value[0];
	    value2 = obj2->value[0];
	    break;

	case ITEM_WEAPON:
	    value1 = obj1->value[1] + obj1->value[2];
	    value2 = obj2->value[1] + obj2->value[2];
	    break;
	}
    }

    if ( msg == NULL )
    {
	     if ( value1 == value2 ) msg = "$p and $P look about the same.";
	else if ( value1  > value2 ) msg = "$p looks better than $P.";
	else                         msg = "$p looks worse than $P.";
    }

    act( msg, ch, obj1, obj2, TO_CHAR );
    return;
}



void do_credits( CHAR_DATA *ch, char *argument )
{
    do_help( ch, "diku" );
    return;
}



void do_wizlist( CHAR_DATA *ch, char *argument )
{
    do_help( ch, "wizlist" );
    return;
}



void do_where( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    DESCRIPTOR_DATA *d;
    AREA_DATA *pArea;
    bool found;

    one_argument( argument, arg );


   if (IS_SET(world_affects, WORLD_FOG))
    {
    send_to_char("It is too foggy to locate anyone.\n\r",ch);
    return;
    }

    pArea = ch->in_room->area;
 
    sprintf(buf, "Name: '%s'\n\r", pArea->name);
    send_to_char(buf, ch);

    if ( arg[0] == '\0' )
    {
	send_to_char( "Players near you:\n\r", ch );
	found = FALSE;
	for ( d = descriptor_list; d != NULL; d = d->next )
	{
	    if ( d->connected == CON_PLAYING
	    && ( victim = d->character ) != NULL
	    &&   !IS_NPC(victim)
	    &&   victim->in_room != NULL
	    &&   victim->in_room->area == ch->in_room->area
	    &&   victim->pcdata->chobj == NULL
	    &&   can_see( ch, victim ) )
	    {
		found = TRUE;
		sprintf( buf, "%-28s %s\n\r",
		    victim->name, victim->in_room->name );
		send_to_char( buf, ch );
	    }
	}
	if ( !found )
	    send_to_char( "None\n\r", ch );
    }
    else
    {
	found = FALSE;
	for ( victim = char_list; victim != NULL; victim = victim->next )
	{
	    if ( victim->in_room != NULL
	    &&   victim->in_room->area == ch->in_room->area
	    &&   !IS_AFFECTED(victim, AFF_HIDE)
	    &&   !IS_AFFECTED(victim, AFF_SNEAK)
	    &&   can_see( ch, victim )
	    &&   is_name( arg, victim->name ) )
	    {
		found = TRUE;
		sprintf( buf, "%-28s %s\n\r",
		    PERS(victim, ch), victim->in_room->name );
		send_to_char( buf, ch );
		break;
	    }
	}
	if ( !found )
	    act( "You didn't find any $T.", ch, NULL, arg, TO_CHAR );
    }

    return;
}




void do_consider( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    char *msg;
    int diff;
    int overall;
    int con_hit;
    int con_dam;
    int con_ac;
    int con_hp;

    one_argument( argument, arg );
    overall = 0;

    if ( arg[0] == '\0' )
    {
	send_to_char( "Consider killing whom?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
	send_to_char( "They're not here.\n\r", ch );
	return;
    }

    act( "You examine $N closely, looking for $S weaknesses.", ch, NULL, victim, TO_CHAR );
    act( "$n examine $N closely, looking for $S weaknesses.", ch, NULL, victim, TO_NOTVICT );
    act( "$n examines you closely, looking for your weaknesses.", ch, NULL, victim, TO_VICT );

    if (!IS_NPC(victim)) do_skill(ch,victim->name);

    if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_VAMPIRE) && 
	IS_EXTRA(victim, EXTRA_FAKE_CON))
    {
	con_hit = victim->pcdata->fake_hit;
	con_dam = victim->pcdata->fake_dam;
	con_ac = victim->pcdata->fake_ac;
	con_hp = victim->pcdata->fake_hp;
    }
    else
    {
	con_hit = char_hitroll(victim);
	con_dam = char_damroll(victim);
	con_ac = char_ac(victim);
	con_hp = victim->hit;
    }
    if (con_hp < 1) con_hp = 1;

    diff = victim->level - ch->level + con_hit - char_hitroll(ch);
         if ( diff <= -35 ) {msg = "You are FAR more skilled than $M."; overall = overall + 3;}
    else if ( diff <= -15 ) {msg = "$E is not as skilled as you are."; overall = overall + 2;}
    else if ( diff <=  -5 ) {msg = "$E doesn't seem quite as skilled as you."; overall = overall + 1;}
    else if ( diff <=   5 ) {msg = "You are about as skilled as $M.";}
    else if ( diff <=  15 ) {msg = "$E is slightly more skilled than you are."; overall = overall - 1;}
    else if ( diff <=  35 ) {msg = "$E seems more skilled than you are."; overall = overall -2;}
    else                    {msg = "$E is FAR more skilled than you."; overall = overall - 3;}
    act( msg, ch, NULL, victim, TO_CHAR );

    diff = victim->level - ch->level + con_dam - char_damroll(ch);
         if ( diff <= -35 ) {msg = "You are FAR more powerful than $M."; overall = overall + 3;}
    else if ( diff <= -15 ) {msg = "$E is not as powerful as you are."; overall = overall + 2;}
    else if ( diff <=  -5 ) {msg = "$E doesn't seem quite as powerful as you."; overall = overall + 1;}
    else if ( diff <=   5 ) {msg = "You are about as powerful as $M.";}
    else if ( diff <=  15 ) {msg = "$E is slightly more powerful than you are."; overall = overall - 1;}
    else if ( diff <=  35 ) {msg = "$E seems more powerful than you are."; overall = overall -2;}
    else                    {msg = "$E is FAR more powerful than you."; overall = overall - 3;}
    act( msg, ch, NULL, victim, TO_CHAR );

    diff = ch->hit * 100 / con_hp;
         if ( diff <=  10 ) {msg = "$E is currently FAR healthier than you are."; overall = overall - 3;}
    else if ( diff <=  50 ) {msg = "$E is currently much healthier than you are."; overall = overall - 2;}
    else if ( diff <=  75 ) {msg = "$E is currently slightly healthier than you are."; overall = overall - 1;}
    else if ( diff <= 125 ) {msg = "$E is currently about as healthy as you are.";}
    else if ( diff <= 200 ) {msg = "You are currently slightly healthier than $M."; overall = overall + 1;}
    else if ( diff <= 500 ) {msg = "You are currently much healthier than $M."; overall = overall + 2;}
    else                    {msg = "You are currently FAR healthier than $M."; overall = overall + 3;}
    act( msg, ch, NULL, victim, TO_CHAR );

    diff = con_ac - char_ac(ch);
         if ( diff <= -100) {msg = "$E is FAR better armoured than you."; overall = overall - 3;}
    else if ( diff <= -50 ) {msg = "$E looks much better armoured than you."; overall = overall - 2;}
    else if ( diff <= -25 ) {msg = "$E looks better armoured than you."; overall = overall - 1;}
    else if ( diff <=  25 ) {msg = "$E seems about as well armoured as you.";}
    else if ( diff <=  50 ) {msg = "You are better armoured than $M."; overall = overall + 1;}
    else if ( diff <=  100) {msg = "You are much better armoured than $M."; overall = overall + 2;}
    else                    {msg = "You are FAR better armoured than $M."; overall = overall + 3;}
    act( msg, ch, NULL, victim, TO_CHAR );

    diff = overall;
         if ( diff <= -11 ) msg = "Conclusion: $E would kill you in seconds.";
    else if ( diff <=  -7 ) msg = "Conclusion: You would need a lot of luck to beat $M.";
    else if ( diff <=  -3 ) msg = "Conclusion: You would need some luck to beat $N.";
    else if ( diff <=   2 ) msg = "Conclusion: It would be a very close fight.";
    else if ( diff <=   6 ) msg = "Conclusion: You shouldn't have a lot of trouble defeating $M.";
    else if ( diff <=  10 ) msg = "Conclusion: $N is no match for you.  You can easily beat $M.";
    else                    msg = "Conclusion: $E wouldn't last more than a few seconds against you.";
    act( msg, ch, NULL, victim, TO_CHAR );

    return;
}

void set_prefix( CHAR_DATA *ch, char *title )
{
    char buf[MAX_STRING_LENGTH];

    if ( IS_NPC(ch) )
    {
	bug( "Set_title: NPC.", 0 );
	return;
    }

    if ( isalpha(title[0]) || isdigit(title[0]) )
    {
	buf[0] = ' ';
	strcpy( buf+1, title );
    }
    else
    {
	strcpy( buf, title );
    }

    free_string( ch->prefix );
    ch->prefix = str_dup( buf );
    return;
}



void do_prefix( CHAR_DATA *ch, char *argument )
{
 
    
    char buf[MAX_STRING_LENGTH];

    if ( IS_NPC(ch) )
	return;

if ( !str_cmp( argument, "none" ))
{
free_string( ch->prefix );
return;
} 
   if ( argument[0] == '\0' )
    {
	send_to_char( "Change your prefix to what?\n\r", ch );
	return;
    }

    if ( strlen(argument) > 13 )
	argument[13] = '\0';

    smash_tilde( argument );
    sprintf(buf,"#n");
    strcpy(buf,argument);
    set_prefix( ch, argument );
    send_to_char( "Prefix Set.\n\r", ch );
}


void set_title( CHAR_DATA *ch, char *title )
{
    char buf[MAX_STRING_LENGTH];

    if ( IS_NPC(ch) )
    {
	bug( "Set_title: NPC.", 0 );
	return;
    }

    if ( isalpha(title[0]) || isdigit(title[0]) )
    {
	buf[0] = ' ';
	strcpy( buf+1, title );
    }
    else
    {
	strcpy( buf, title );
    }

    free_string( ch->pcdata->title );
    ch->pcdata->title = str_dup( buf );
    return;
}

void set_titlec( CHAR_DATA *ch, char *titlec )
{
    char buf[MAX_STRING_LENGTH];

    if ( IS_NPC(ch) )
    {
        bug( "Set_titlec: NPC.", 0 );
        return;
    }
     
    if ( isalpha(titlec[0]) || isdigit(titlec[0]) )
    {
        buf[0] = ' ';
        strcpy( buf+1, titlec );
    }
    else
    {
        strcpy( buf, titlec );  
    }
        
    free_string( ch->pcdata->titlec );
    ch->pcdata->titlec = str_dup( buf );
    return;
}

void do_title( CHAR_DATA *ch, char *argument )
{
    int i;
    if ( IS_NPC(ch) )
	return;

    if ( argument[0] == '\0' )
    {
	send_to_char( "Change your title to what?\n\r", ch );
	return;
    }

    for ( i = 0; i < strlen(argument); i++ )
    {
        if (( argument[i] == '#' ) || (argument[i] == '{' ) ||
            (argument[i] == '`' ) )
	{
	    stc("You cannot use color in titles.\n\r",ch);
	    return;
	}
    }

    if ( strlen(argument) > 15 )
    {
	argument[13] = ' ';
        argument[14] = '\0';
    }

    smash_tilde( argument );
    set_title( ch, argument );
    send_to_char( "Ok.\n\r", ch );
}

void do_classtitle( CHAR_DATA *ch, char *argument )
{
    if ( IS_NPC(ch) )
        return;
    
    if (ch->pcdata->quest < 500)
    { stc("It costs 500 qp to change your class title.\n\r",ch); return; }

    if (ch->class_2 == 0)
    {
      stc("You're not multiclassed. Moron!\n\r",ch); return; }

    if ( argument[0] == '\0' )
    {
        send_to_char( "Change your class title to what?\n\r", ch );
        return;
    }   
      
    if ( strlen(argument) > 65 )
    {
        argument[64] = ' ';
        argument[65] = '\0';
    }

    smash_tilde( argument );  
    set_titlec( ch, argument );
    send_to_char( "Ok.\n\r", ch );
}

void do_afk( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];

	if ( IS_NPC(ch) )
	return;

        if (IS_SET(ch->flag2, EXTRA_AFK))
	{
	free_string( ch->pcdata->title);
	ch->pcdata->title = str_dup( ch->short_descr );
	free_string( ch->short_descr );
	ch->short_descr = NULL;
	send_to_char( "You are no longer AFK.\n\r", ch );
	sprintf(buf,"%s is no longer AFK!",ch->name);
      ADD_COLOUR(ch,buf,L_RED);
      if( IS_IMMORTAL(ch) && IS_SET( ch->act, PLR_WIZINVIS ) )
          do_immtalk(ch, buf);
      else
          do_info(ch, buf);
	REMOVE_BIT(ch->flag2,EXTRA_AFK);
        return;
	}
	
	else if (!IS_SET(ch->flag2,EXTRA_AFK))
	{
	free_string( ch->short_descr );
	ch->short_descr = str_dup( ch->pcdata->title );
	free_string( ch->pcdata->title );
	ch->pcdata->title = str_dup("..IS AFK");
	send_to_char( "You are now AFK.\n\r", ch );
	sprintf(buf,"%s is now AFK!",ch->name);
	ADD_COLOUR(ch,buf,L_RED);
      if( IS_IMMORTAL(ch) && IS_SET( ch->act, PLR_WIZINVIS ) )
          do_immtalk(ch, buf);
      else
          do_info(ch, buf);
	SET_BIT(ch->flag2,EXTRA_AFK);
	WAIT_STATE(ch, 25);
        return;
	}

else
return;

}



void do_email( CHAR_DATA *ch, char *argument )
{
    if ( IS_NPC(ch) )
	return;

    if ( argument[0] == '\0' )
    {
	send_to_char( "What do you wish to set your email address to?\n\r", ch );
	return;
    }

    if ( strlen(argument) > 50 )
	argument[50] = '\0';

    smash_tilde( argument );
    free_string( ch->pcdata->email );
    ch->pcdata->email = str_dup( argument );
    send_to_char( "Ok.\n\r", ch );
}



void do_description( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    int i;

    if ( argument[0] != '\0' )
    {
        for ( i = 0; i < strlen(argument); i++ )
	{
	    if (( argument[i] == '#' ) || (argument[i] == '{' ) ||
                ( argument[i] == '`' ) )
	    {
	        send_to_char("No color in that please.\n\r",ch);
	        return;
	    }
        }
    }

    if ( argument[0] != '\0' )
    {
	buf[0] = '\0';
	smash_tilde( argument );
	if ( argument[0] == '+' )
	{
	    if ( ch->description != NULL )
		strcat( buf, ch->description );
	    argument++;
	    while ( isspace(*argument) )
		argument++;
	}

	if ( strlen(buf) + strlen(argument) >= MAX_STRING_LENGTH - 2 )
	{
	    send_to_char( "Description too long.\n\r", ch );
	    return;
	}

	strcat( buf, argument );
	strcat( buf, "\n\r" );
	free_string( ch->description );
	ch->description = str_dup( buf );
    }

    send_to_char( "Your description is:\n\r", ch );
    send_to_char( ch->description ? ch->description : "(None).\n\r", ch );
    return;
}



void do_report( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *vch;
    CHAR_DATA *vch_next;
    char buf[MAX_STRING_LENGTH];
    char hit_str[MAX_INPUT_LENGTH];
    char mana_str[MAX_INPUT_LENGTH];
    char move_str[MAX_INPUT_LENGTH];
    char mhit_str[MAX_INPUT_LENGTH];
    char mmana_str[MAX_INPUT_LENGTH];
    char mmove_str[MAX_INPUT_LENGTH];
    char exp_str[MAX_INPUT_LENGTH];
    sprintf(hit_str, "%ld", ch->hit);
    COL_SCALE(hit_str, ch, ch->hit, ch->max_hit);
    sprintf(mana_str, "%ld", ch->mana);
    COL_SCALE(mana_str, ch, ch->mana, ch->max_mana);
    sprintf(move_str, "%ld", ch->move);
    COL_SCALE(move_str, ch, ch->move, ch->max_move);
    sprintf(exp_str, "%d", ch->exp);
    COL_SCALE(exp_str, ch, ch->exp, 1000);
    sprintf(mhit_str, "%ld", ch->max_hit);
    ADD_COLOUR(ch, mhit_str, L_CYAN);
    sprintf(mmana_str, "%ld", ch->max_mana);
    ADD_COLOUR(ch, mmana_str, L_CYAN);
    sprintf(mmove_str, "%ld", ch->max_move);
    ADD_COLOUR(ch, mmove_str, L_CYAN);
    sprintf( buf,
	"You report: %s/%s hp %s/%s mana %s/%s mv %s xp.\n\r",
	hit_str,  mhit_str,
	mana_str, mmana_str,
	move_str, mmove_str,
	exp_str   );

    send_to_char( buf, ch );

    for ( vch = char_list; vch != NULL; vch = vch_next )
    {
	vch_next	= vch->next;
	if ( vch == NULL ) continue;
	if ( vch == ch ) continue;
	if ( vch->in_room == NULL ) continue;
	if ( vch->in_room != ch->in_room ) continue;
    	sprintf(hit_str, "%ld", ch->hit);
    	COL_SCALE(hit_str, vch, ch->hit, ch->max_hit);
    	sprintf(mana_str, "%ld", ch->mana);
    	COL_SCALE(mana_str, vch, ch->mana, ch->max_mana);
    	sprintf(move_str, "%ld", ch->move);
    	COL_SCALE(move_str, vch, ch->move, ch->max_move);
    	sprintf(exp_str, "%d", ch->exp);
    	COL_SCALE(exp_str, vch, ch->exp, 1000);
    	sprintf(mhit_str, "%ld", ch->max_hit);
    	ADD_COLOUR(vch, mhit_str, L_CYAN);
    	sprintf(mmana_str, "%ld", ch->max_mana);
    	ADD_COLOUR(vch, mmana_str, L_CYAN);
    	sprintf(mmove_str, "%ld", ch->max_move);
    	ADD_COLOUR(vch, mmove_str, L_CYAN);
    	if (!IS_NPC(ch) && IS_AFFECTED(ch,AFF_POLYMORPH))
    	    sprintf( buf, "%s reports: %s/%s hp %s/%s mana %s/%s mv %s xp.\n\r",
	    ch->morph,
	    hit_str,  mhit_str,
	    mana_str, mmana_str,
	    move_str, mmove_str,
	    exp_str   );
        else
    	    sprintf( buf, "%s reports: %s/%s hp %s/%s mana %s/%s mv %s xp.\n\r",
	    IS_NPC(ch) ? capitalize(ch->short_descr) : ch->name,
	    hit_str,  mhit_str,
	    mana_str, mmana_str,
	    move_str, mmove_str,
	    exp_str   );
	buf[0] = UPPER(buf[0]);
    	send_to_char( buf, vch );
    }
    return;
}



void do_practice( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    int sn;

    if ( IS_NPC(ch) )
	return;

    if ( argument[0] == '\0' )
    {
	int col;
        int num_skills_displayed;

	col    = 0;
        num_skills_displayed = 0;
	for ( sn = 0; sn < MAX_SKILL; sn++ )
	{
	    if ( skill_table[sn].name == NULL )
		break;

  // Simian -=====- 28-03-00
  // Prac now only shows players the skills known
        
            if ( ch->pcdata->learned[sn] >= 1 )
               {
	       if ( ch->level < skill_table[sn].skill_level )
	          continue;
	             sprintf( buf, "%18s %3d%%  ",
		  skill_table[sn].name, ch->pcdata->learned[sn] );
	          send_to_char( buf, ch );
	       if ( ++col % 3 == 0 )
		  send_to_char( "\n\r", ch );
                  num_skills_displayed++;
               }	         
	}

        if ( num_skills_displayed == 0 )
           send_to_char( "You have yet to practice anything!\n\r", ch );
	if ( col % 3 != 0 )
	    send_to_char( "\n\r", ch );

        send_to_char( "Type {WPLIST {wto see the the skills and spells you have not yet learned.\n\r", ch );

	sprintf( buf, "You have %d exp left.\n\r", ch->exp );
	send_to_char( buf, ch );
    }
    else
    {
	if ( !IS_AWAKE(ch) )
	{
	    send_to_char( "In your dreams, or what?\n\r", ch );
	    return;
	}

	if ( ch->exp <= 0 )
	{
	    send_to_char( "You have no exp left.\n\r", ch );
	    return;
	}


	if ( ( sn = skill_lookup( argument ) ) < 0 || ( !IS_NPC(ch) && ch->level < skill_table[sn].skill_level ) )
	{
	    send_to_char( "You can't practice that.\n\r", ch );
	    return;
	}

//-=====-Simian-13-04-00
// Stops training when each class reaches its cap

       if ( ch->class == CLASS_VAMPIRE )
           {
           if ( ch->pcdata->learned[sn] >= 100 )
                        {
                        act( "You already know $T as well as a vampire can.",
                        ch, NULL, skill_table[sn].name, TO_CHAR );
                        return;
                        }
           }
       if ( ch->class == CLASS_WEREWOLF )
           {             
           if ( ch->pcdata->learned[sn] >= 100 )
                        {
                        act( "You already know $T as well as a werewolf can.",
                        ch, NULL, skill_table[sn].name, TO_CHAR );
                        return; 
                        }
           }
       if ( ch->class == CLASS_DEMON )
           {             
           if ( ch->pcdata->learned[sn] >= 100 )
                        {
                        act( "You already know $T as well as a demon can.",
                        ch, NULL, skill_table[sn].name, TO_CHAR );
                        return; 
                        }
           }
       if ( ch->class == CLASS_NINJA )
           {             
           if ( ch->pcdata->learned[sn] >= 100 )
                        {
                        act( "You already know $T as well as a ninja can.",
                        ch, NULL, skill_table[sn].name, TO_CHAR );
                        return; 
                        }
           }
       if ( ch->class == CLASS_MONK )
           {             
           if ( ch->pcdata->learned[sn] >= 100 )
                        {
                        act( "You already know $T as well as a monk can.",
                        ch, NULL, skill_table[sn].name, TO_CHAR );
                        return; 
                        }
           }
       if ( ch->class == CLASS_DROW )
           {             
           if ( ch->pcdata->learned[sn] >= 100 )
                        {
                        act( "You already know $T as well as a drow can.",
                        ch, NULL, skill_table[sn].name, TO_CHAR );
                        return; 
                        }
           }
       if ( ch->class == CLASS_HIGHLANDER )
           {             
           if ( ch->pcdata->learned[sn] >= 100 )
                        {
                        act( "You already know $T as well as a highlander can.",
                        ch, NULL, skill_table[sn].name, TO_CHAR );
                        return; 
                        }
           }
       if ( ch->class == CLASS_MAGE )
           {
           switch ( ch->level )
              {
              default:
              case LEVEL_APPRENTICE: 
                 if ( ch->pcdata->learned[sn] >= 100 )
                    {
                    act( "You already know $T as well as an apprentice can.",
                    ch, NULL, skill_table[sn].name, TO_CHAR );
                    break;
                    }
              case LEVEL_MAGE:
                 if ( ch->pcdata->learned[sn] >= 100 )
                    {
                    act( "You already know $T as well as a mage can.",   
                    ch, NULL, skill_table[sn].name, TO_CHAR );
                    break;
                    }
               case LEVEL_ARCHMAGE:
                  if ( ch->pcdata->learned[sn] == SKILL_ADEPT )
                     {
                     act( "You already know $T as well as an archmage can.",
                     ch, NULL, skill_table[sn].name, TO_CHAR );
                     break;
                     }
                }
             }

        if( ch->class == CLASS_NONE )
           {
           if ( ch->pcdata->learned[sn] >= 100 )
              {
              act("You already know $T as well as a mortal can.",
              ch, NULL, skill_table[sn].name, TO_CHAR );
              return;
              }
           }

	if ( ch->pcdata->learned[sn] >= SKILL_ADEPT )
	{
	    sprintf( buf, "You are already an adept of %s.\n\r",
	         skill_table[sn].name );
	    send_to_char( buf, ch );
            return;
	}
        if ( ch->exp < 5000 )
	{
	    sprintf( buf, "You need 5000 exp to learn %s.\n\r",
		skill_table[sn].name );
	    send_to_char( buf, ch );
	}
	else
	{
	    	ch->exp -= 5000;
		     if ( ch->class == CLASS_VAMPIRE )
		        {
                        ch->pcdata->learned[sn] = 100;
                        act( "You are now trained in the ways of $T.",
                        ch, NULL, skill_table[sn].name, TO_CHAR );
	         	}
	             else if ( ch->class == CLASS_WEREWOLF )
                           {
                           ch->pcdata->learned[sn] = 100;         
                           act( "You are now trained in the ways of $T.",
                           ch, NULL, skill_table[sn].name, TO_CHAR );
                           return;
                           }
	             else if ( ch->class == CLASS_DEMON )
                           {
                           ch->pcdata->learned[sn] = 100;         
                           act( "You are now trained in the ways of $T.",
                           ch, NULL, skill_table[sn].name, TO_CHAR );
                           return;
                           } 
             	      else if ( ch->class == CLASS_NINJA )
                           {
                           ch->pcdata->learned[sn] = 100;
                           act( "You are now trained in the ways of $T.",
                           ch, NULL, skill_table[sn].name, TO_CHAR );
                           return;
                           }
	              else if ( ch->class == CLASS_MONK )
                           {
                           ch->pcdata->learned[sn] = 100;         
                           act( "You are now trained in the ways of $T.",
                           ch, NULL, skill_table[sn].name, TO_CHAR );
                           return;
                           }
 		      else if ( ch->class == CLASS_DROW )
                           {
                           ch->pcdata->learned[sn] = 100;         
                           act( "You are now trained in the ways of $T.",
                           ch, NULL, skill_table[sn].name, TO_CHAR );
                           return;
                           }
	              else if ( ch->class == CLASS_HIGHLANDER )
                           {
                           ch->pcdata->learned[sn] = 100;         
                           act( "You are now trained in the ways of $T.",
                           ch, NULL, skill_table[sn].name, TO_CHAR );
                           return;
                           } 
	    	           else if ( ch->class == CLASS_MAGE )
		              {
                              switch ( ch->level )
                                 { 
                                 default:
                                 case LEVEL_APPRENTICE:
                                    ch->pcdata->learned[sn] = 100;
                                    act( "You are now trained of $T.",
                                    ch, NULL, skill_table[sn].name, TO_CHAR );
                                    break;
                                 case LEVEL_MAGE:
                                    ch->pcdata->learned[sn] = 100;
                                    act( "You are now trained of $T.",
                                    ch, NULL, skill_table[sn].name, TO_CHAR );
                                    break;
                                 case LEVEL_ARCHMAGE:
                                    ch->pcdata->learned[sn] = SKILL_ADEPT;
                                    act( "You are now an adept of $T.",
                                    ch, NULL, skill_table[sn].name, TO_CHAR );
	                            break;
		                  }
                               return;
	                       }
	  	else if( ch->class == CLASS_NONE )
		   {	
		   ch->pcdata->learned[sn] = 100;
		   act("You get the basic knowledge of $T.",
		   ch, NULL, skill_table[sn].name, TO_CHAR );
                   return;
		   }

	}
   }    
return;
}

// Simian -=====- 30-03-00
// PLIST command added to show players all unpracticed skills/spells

void do_plist( CHAR_DATA *ch, char *argument )
{

   int sn;
   char buf[MAX_STRING_LENGTH];
   int num_skills_displayed;
   int col;

   if ( IS_NPC( ch ) )
      return;

   if ( argument[0] == '\0' )
      ;
   else
      ;
            
   col    = 0;
   num_skills_displayed = 0;
   for ( sn = 0; sn < MAX_SKILL; sn++ )
      {  
      if ( skill_table[sn].name == NULL )
         break;
      if ( ch->pcdata->learned[sn] == 0 )
               {
               if ( ch->level < skill_table[sn].skill_level )
                  continue;
               sprintf( buf, "%18s %3d%%  ",
               skill_table[sn].name, ch->pcdata->learned[sn] );
               send_to_char( buf, ch );
               if ( ++col % 3 == 0 )
                  send_to_char( "\n\r", ch );
               num_skills_displayed++;
               }
        }
  
        if ( num_skills_displayed == 0 )
           send_to_char( "You have already practiced everything!\n\r", ch );
        if ( col % 3 != 0 )
            send_to_char( "\n\r", ch );
         
        send_to_char( "Type {WPRACTICE {wto see the skills and spells you know.\n\r", ch );

}





/*
 * 'Wimpy' originally by Dionysos.
 */
void do_wimpy( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    int wimpy;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
	wimpy = ch->max_hit / 5;
    else
	wimpy = atoi( arg );

    if ( wimpy < 0 )
    {
	send_to_char( "Your courage exceeds your wisdom.\n\r", ch );
	return;
    }

    if ( wimpy > ch->max_hit )
    {
	send_to_char( "Such cowardice ill becomes you.\n\r", ch );
	return;
    }

    ch->wimpy	= wimpy;
    sprintf( buf, "Wimpy set to %d hit points.\n\r", wimpy );
    send_to_char( buf, ch );
    return;
}



void do_password( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    char *pArg;
    char *pwdnew;
    char *p;
    char cEnd;

    if ( IS_NPC(ch) )
	return;

    /*
     * Can't use one_argument here because it smashes case.
     * So we just steal all its code.  Bleagh.
     */
    pArg = arg1;
    while ( isspace(*argument) )
	argument++;

    cEnd = ' ';
    if ( *argument == '\'' || *argument == '"' )
	cEnd = *argument++;

    while ( *argument != '\0' )
    {
	if ( *argument == cEnd )
	{
	    argument++;
	    break;
	}
	*pArg++ = *argument++;
    }
    *pArg = '\0';

    pArg = arg2;
    while ( isspace(*argument) )
	argument++;

    cEnd = ' ';
    if ( *argument == '\'' || *argument == '"' )
	cEnd = *argument++;

    while ( *argument != '\0' )
    {
	if ( *argument == cEnd )
	{
	    argument++;
	    break;
	}
	*pArg++ = *argument++;
    }
    *pArg = '\0';

    if ( arg1[0] == '\0' || arg2[0] == '\0' )
    {
	send_to_char( "Syntax: password <old> <new>.\n\r", ch );
	return;
    }

    if ( strcmp( arg1, ch->pcdata->pwd ) &&
         strcmp( crypt( arg1, ch->pcdata->pwd ), ch->pcdata->pwd ) )
    {
	WAIT_STATE( ch, 40 );
	send_to_char( "Wrong password.  Wait 10 seconds.\n\r", ch );
	return;
    }

   if ( strlen(arg2) < 5 )
    {
	send_to_char(
	    "New password must be at least five characters long.\n\r", ch );
	return;
    }

    /*
     * No tilde allowed because of player file format.
     */
    pwdnew = crypt( arg2, ch->name );
    for ( p = pwdnew; *p != '\0'; p++ )
    {
	if ( *p == '~' )
	{
	    send_to_char(
		"New password not acceptable, try again.\n\r", ch );
	    return;
	}
    }

    free_string( ch->pcdata->pwd );
    ch->pcdata->pwd = str_dup( pwdnew );
    if (!IS_EXTRA(ch,EXTRA_NEWPASS)) SET_BIT(ch->extra,EXTRA_NEWPASS);
    save_char_obj( ch );
    if (ch->desc != NULL && ch->desc->connected == CON_PLAYING )
	send_to_char( "Ok.\n\r", ch );
    return;
}



void do_socials( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    int iSocial;
    int col;

    col = 0;

    for ( iSocial = 0; social_table[iSocial].name[0] != '\0'; iSocial++ )
    {
	sprintf( buf, "%-12s", social_table[iSocial].name );
	send_to_char( buf, ch );
	if ( ++col % 6 == 0 )
	    send_to_char( "\n\r", ch );
    }
 
    if ( col % 6 != 0 )
	send_to_char( "\n\r", ch );

    return;
}



void do_xsocials( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    int iSocial;
    int col;

    col = 0;

    for ( iSocial = 0; xsocial_table[iSocial].name[0] != '\0'; iSocial++ )
    {
	sprintf( buf, "%-12s", xsocial_table[iSocial].name );
	send_to_char( buf, ch );
	if ( ++col % 6 == 0 )
	    send_to_char( "\n\r", ch );
    }
 
    if ( col % 6 != 0 )
	send_to_char( "\n\r", ch );

    return;
}



void do_spells( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    int sn;
    int col;

    col = 0;
    for ( sn = 0; sn < MAX_SKILL && skill_table[sn].name != NULL; sn++ )
    {
	sprintf( buf, "%-12s", skill_table[sn].name );
	send_to_char( buf, ch );
	if ( ++col % 6 == 0 )
	    send_to_char( "\n\r", ch );
    }

    if ( col % 6 != 0 )
	send_to_char( "\n\r", ch );
    return;
}



/*
 * Contributed by Alander.
 */
void do_commands( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    int cmd;
    int col;
 
    col = 0;
    for ( cmd = 0; cmd_table[cmd].name[0] != '\0'; cmd++ )
    {
        if ( cmd_table[cmd].level == 0
        &&   cmd_table[cmd].level <= get_trust( ch ) )
	{
	    sprintf( buf, "%-12s", cmd_table[cmd].name );
	    send_to_char( buf, ch );
	    if ( ++col % 6 == 0 )
		send_to_char( "\n\r", ch );
	}
    }
 
    if ( col % 6 != 0 )
	send_to_char( "\n\r", ch );
    return;
}



void do_channels( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];

    one_argument( argument, arg );

    if (IS_NPC(ch)) return;
    if ( arg[0] == '\0' )
    {
	if ( !IS_NPC(ch) && IS_SET(ch->act, PLR_SILENCE) )
	{
	    send_to_char( "You are silenced.\n\r", ch );
	    return;
	}

	send_to_char( "Channels:", ch );

/*	send_to_char( !IS_SET(ch->deaf, CHANNEL_AUCTION)
	    ? " +AUCTION"
	    : " -auction",
	    ch );
*/
	send_to_char( !IS_SET(ch->deaf, CHANNEL_CHAT)
	    ? " +CHAT"
	    : " -chat",
	    ch );

#if 0
	send_to_char( !IS_SET(ch->deaf, CHANNEL_HACKER)
	    ? " +HACKER"
	    : " -hacker",
	    ch );
#endif

	if ( IS_IMMORTAL(ch) )
	{
	    send_to_char( !IS_SET(ch->deaf, CHANNEL_IMMTALK)
		? " +IMMTALK"
		: " -immtalk",
		ch );
	}
	
	send_to_char( !IS_SET(ch->deaf, CHANNEL_MUSIC)
	    ? " +MUSIC"
	    : " -music",
	    ch );

	send_to_char( !IS_SET(ch->deaf, CHANNEL_QUESTION)
	    ? " +QUESTION"
	    : " -question",
	    ch );

	send_to_char( !IS_SET(ch->deaf, CHANNEL_SHOUT)
	    ? " +SHOUT"
	    : " -shout",
	    ch );

        send_to_char( !IS_SET(ch->deaf, CHANNEL_HINT)
            ? " +HINT"
            : " -hint",
            ch );

	send_to_char( !IS_SET(ch->deaf, CHANNEL_HOWL)
	    ? " +HOWL"
	    : " -howl",
	    ch );

	if (IS_IMMORTAL(ch))
	{
	    send_to_char( !IS_SET(ch->deaf, CHANNEL_LOG)
		? " +LOG"
		: " -log",
		ch );
	}

    if (IS_CLASS( ch, CLASS_MAGE) || IS_IMMORTAL(ch))
    {
        send_to_char( !IS_SET(ch->deaf, CHANNEL_MAGETALK)
        ? " +MAGE"
        : " -mage",
        ch );
    }
        if (IS_CLASS( ch, CLASS_MONK) || IS_IMMORTAL(ch))
        {
            send_to_char( !IS_SET(ch->deaf, CHANNEL_MONK)
                ? " +MONK"
                : " -monk",
                ch );
        }

	if (IS_CLASS(ch, CLASS_DEMON) || IS_IMMORTAL(ch))
	{
	    send_to_char( !IS_SET(ch->deaf, CHANNEL_PRAY)
		? " +PRAY"
		: " -pray",
		ch );
	}
	
    if (IS_CLASS( ch, CLASS_NINJA) || IS_IMMORTAL(ch))
    {
        send_to_char( !IS_SET(ch->deaf, CHANNEL_MIKT)
        ? " +MIKT"
        : " -mikt",
        ch );
    }
       
        send_to_char( !IS_SET(ch->deaf, CHANNEL_BITCH)
        ? " +BITCH"
        : " -bitch",
        ch );

	send_to_char( !IS_SET(ch->deaf, CHANNEL_INFO)
	    ? " +INFO"
	    : " -info",
	    ch );

	if (IS_CLASS(ch, CLASS_VAMPIRE) || IS_IMMORTAL(ch))
	{
	    send_to_char( !IS_SET(ch->deaf, CHANNEL_VAMPTALK)
		? " +VAMP"
		: " -vamp",
		ch );
	}

	if (IS_CLASS(ch, CLASS_HIGHLANDER) || IS_IMMORTAL(ch)) {
		send_to_char( !IS_SET(ch->deaf, CHANNEL_HIGHTALK)
			? " +HIGH"
			: " -high",
			ch ); }

	if (IS_CLASS(ch, CLASS_DROW) || IS_IMMORTAL(ch)) {
	send_to_char(!IS_SET(ch->deaf, CHANNEL_SIGN)
		? " +SIGN"
		: " -sign",
		ch ); }

	send_to_char( !IS_SET(ch->deaf, CHANNEL_TELL)
	    ? " +TELL"
	    : " -tell",
	    ch );

	send_to_char( ".\n\r", ch );
    }
    else
    {
	bool fClear;
	int bit;

	     if ( arg[0] == '+' ) fClear = TRUE;
	else if ( arg[0] == '-' ) fClear = FALSE;
	else
	{
	    send_to_char( "Channels -channel or +channel?\n\r", ch );
	    return;
	}

/*	     if ( !str_cmp( arg+1, "auction"  ) ) bit = CHANNEL_AUCTION;*/
        if ( !str_cmp( arg+1, "chat"     ) ) bit = CHANNEL_CHAT;
#if 0
	else if ( !str_cmp( arg+1, "hacker"   ) ) bit = CHANNEL_HACKER;
#endif
	else if ( !str_cmp( arg+1, "immtalk"  ) ) bit = CHANNEL_IMMTALK;
	else if ( !str_cmp( arg+1, "high"     ) ) bit = CHANNEL_HIGHTALK;
	else if ( !str_cmp( arg+1, "sign"     ) ) bit = CHANNEL_SIGN;
	else if ( !str_cmp( arg+1, "music"    ) ) bit = CHANNEL_MUSIC;
	else if ( !str_cmp( arg+1, "question" ) ) bit = CHANNEL_QUESTION;
	else if ( !str_cmp( arg+1, "shout"    ) ) bit = CHANNEL_SHOUT;
	else if ( !str_cmp( arg+1, "yell"     ) ) bit = CHANNEL_YELL;
	else if ( !str_cmp( arg+1, "mikt"  ) ) bit = CHANNEL_MIKT;
	else if ( !str_cmp( arg+1, "bitch"  ) ) bit = CHANNEL_BITCH;
	else if ( !str_cmp( arg+1, "howl"     ) ) bit = CHANNEL_HOWL;
	else if (IS_IMMORTAL(ch) && !str_cmp( arg+1, "log") ) bit = CHANNEL_LOG;
	else if ( !str_cmp( arg+1, "monk"     ) ) bit = CHANNEL_MONK;
        else if ( !str_cmp( arg+1, "mage"     ) ) bit = CHANNEL_MAGETALK;
	else if ((IS_SET(ch->special, SPC_CHAMPION) || (IS_IMMORTAL(ch))) && !str_cmp( arg+1, "pray") ) 
	    bit = CHANNEL_PRAY;
	else if ( !str_cmp( arg+1, "info"     ) ) bit = CHANNEL_INFO;
	else if ( !str_cmp( arg+1, "vamp"     ) ) bit = CHANNEL_VAMPTALK;
	else if ( !str_cmp( arg+1, "tell"     ) ) bit = CHANNEL_TELL;
        else if ( !str_cmp( arg+1, "hint"     ) ) bit = CHANNEL_HINT;
	else
	{
	    send_to_char( "Set or clear which channel?\n\r", ch );
	    return;
	}

	if ( fClear )
	    REMOVE_BIT (ch->deaf, bit);
	else
	    SET_BIT    (ch->deaf, bit);

	send_to_char( "Ok.\n\r", ch );
    }

    return;
}



/*
 * Contributed by Grodyn.
 */
void do_config( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];

    if ( IS_NPC(ch) )
	return;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
        send_to_char( "[ Keyword  ] Option\n\r", ch );

	send_to_char(  IS_SET(ch->act, PLR_ANSI)
            ? "[+ANSI     ] You have ansi colour on.\n\r"
	    : "[-ansi     ] You have ansi colour off.\n\r"
	    , ch );

	send_to_char(  IS_SET(ch->act, PLR_AUTOEXIT)
            ? "[+AUTOEXIT ] You automatically see exits.\n\r"
	    : "[-autoexit ] You don't automatically see exits.\n\r"
	    , ch );

	send_to_char(  IS_SET(ch->act, PLR_AUTOLOOT)
	    ? "[+AUTOLOOT ] You automatically loot corpses.\n\r"
	    : "[-autoloot ] You don't automatically loot corpses.\n\r"
	    , ch );

	send_to_char(  IS_SET(ch->act, PLR_AUTOSAC)
	    ? "[+AUTOSAC  ] You automatically sacrifice corpses.\n\r"
	    : "[-autosac  ] You don't automatically sacrifice corpses.\n\r"
	    , ch );

        if (IS_CLASS(ch, CLASS_DRAGON)) {
        send_to_char(  IS_SET(ch->newbits, PLR_AUTOCON)
            ? "[+AUTOCON  ] You automatically consume corpses.\n\r"
            : "[-autocon  ] You don't automatically consume corpses.\n\r"
            , ch ); }

	send_to_char(  IS_SET(ch->act, PLR_BLANK)
	    ? "[+BLANK    ] You have a blank line before your prompt.\n\r"
	    : "[-blank    ] You have no blank line before your prompt.\n\r"
	    , ch );

	send_to_char(  IS_SET(ch->act, PLR_BRIEF)
	    ? "[+BRIEF    ] You see brief descriptions.\n\r"
	    : "[-brief    ] You see long descriptions.\n\r"
	    , ch );
         
	send_to_char(  IS_SET(ch->act, PLR_COMBINE)
	    ? "[+COMBINE  ] You see object lists in combined format.\n\r"
	    : "[-combine  ] You see object lists in single format.\n\r"
	    , ch );

	send_to_char(  IS_SET(ch->act, PLR_PROMPT)
	    ? "[+PROMPT   ] You have a prompt.\n\r"
	    : "[-prompt   ] You don't have a prompt.\n\r"
	    , ch );

	send_to_char(  IS_SET(ch->act, PLR_TELNET_GA)
	    ? "[+TELNETGA ] You receive a telnet GA sequence.\n\r"
	    : "[-telnetga ] You don't receive a telnet GA sequence.\n\r"
	    , ch );

	send_to_char(  IS_SET(ch->act, PLR_SILENCE)
	    ? "[+SILENCE  ] You are silenced.\n\r"
	    : ""
	    , ch );

	send_to_char( !IS_SET(ch->act, PLR_NO_EMOTE)
	    ? ""
	    : "[-emote    ] You can't emote.\n\r"
	    , ch );

	send_to_char( !IS_SET(ch->act, PLR_NO_TELL)
	    ? ""
	    : "[-tell     ] You can't use 'tell'.\n\r"
	    , ch );
    }
    else
    {
	bool fSet;
	int bit = 0;

	     if ( arg[0] == '+' ) fSet = TRUE;
	else if ( arg[0] == '-' ) fSet = FALSE;
	else
	{
	    send_to_char( "Config -option or +option?\n\r", ch );
	    return;
	}

             if ( !str_cmp( arg+1, "ansi"     ) ) bit = PLR_ANSI;
        else if ( !str_cmp( arg+1, "autoexit" ) ) bit = PLR_AUTOEXIT;
	else if ( !str_cmp( arg+1, "autoloot" ) ) bit = PLR_AUTOLOOT;
	else if ( !str_cmp( arg+1, "autosac"  ) ) bit = PLR_AUTOSAC;
	else if ( !str_cmp( arg+1, "blank"    ) ) bit = PLR_BLANK;
	else if ( !str_cmp( arg+1, "brief"    ) ) bit = PLR_BRIEF;
	else if ( !str_cmp( arg+1, "combine"  ) ) bit = PLR_COMBINE;
        else if ( !str_cmp( arg+1, "prompt"   ) ) bit = PLR_PROMPT;
	else if ( !str_cmp( arg+1, "telnetga" ) ) bit = PLR_TELNET_GA;
        else if ( !str_cmp( arg+1, "autocon"  ) ) { 
        if ( fSet )
            SET_BIT(ch->newbits, PLR_AUTOCON);
        else
            REMOVE_BIT(ch->newbits, PLR_AUTOCON);
         }
	else
	{
	    send_to_char( "Config which option?\n\r", ch );
	    return;
	}

        if ( fSet ) 
            SET_BIT    (ch->act, bit);
        else
            REMOVE_BIT (ch->act, bit);

	send_to_char( "Ok.\n\r", ch );
    }

    return;
}

void do_ansi( CHAR_DATA *ch, char *argument )
{
    if ( IS_NPC(ch) ) return;
    if (IS_SET(ch->act, PLR_ANSI)) do_config(ch,"-ansi");
    else do_config(ch,"+ansi");
    return;
}

void do_autoexit( CHAR_DATA *ch, char *argument )
{
    if ( IS_NPC(ch) ) return;
    if (IS_SET(ch->act, PLR_AUTOEXIT)) do_config(ch,"-autoexit");
    else do_config(ch,"+autoexit");
    return;
}

void do_autoloot( CHAR_DATA *ch, char *argument )
{
    if ( IS_NPC(ch) ) return;
    if (IS_SET(ch->act, PLR_AUTOLOOT)) do_config(ch,"-autoloot");
    else do_config(ch,"+autoloot");
    return;
}

void do_autosac( CHAR_DATA *ch, char *argument )
{
    if ( IS_NPC(ch) ) return;
    if (IS_SET(ch->act, PLR_AUTOSAC)) do_config(ch,"-autosac");
    else do_config(ch,"+autosac");
    return;
}

void do_blank( CHAR_DATA *ch, char *argument )
{
    if ( IS_NPC(ch) ) return;
    if (IS_SET(ch->act, PLR_BLANK)) do_config(ch,"-blank");
    else do_config(ch,"+blank");
    return;
}

void do_brief( CHAR_DATA *ch, char *argument )
{
    if ( IS_NPC(ch) ) return;
    if (IS_SET(ch->act, PLR_BRIEF)) do_config(ch,"-brief");
    else do_config(ch,"+brief");
    return;
}

void do_diagnose( CHAR_DATA *ch, char *argument )
{
    char buf  [MAX_STRING_LENGTH];
    char arg  [MAX_INPUT_LENGTH];
    int teeth = 0;
    int ribs = 0;
    CHAR_DATA *victim;

    argument = one_argument( argument, arg );

    if (arg == '\0')
    {
	send_to_char("Who do you wish to diagnose?\n\r",ch);
	return;
    }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
	send_to_char("Nobody here by that name.\n\r",ch);
	return;
    }
    act("$n examines $N carefully, diagnosing $S injuries.",ch,NULL,victim,TO_NOTVICT);
    act("$n examines you carefully, diagnosing your injuries.",ch,NULL,victim,TO_VICT);
    act("Your diagnoses of $N reveals the following...",ch,NULL,victim,TO_CHAR);
    send_to_char("--------------------------------------------------------------------------------\n\r",ch);
    if ( ( victim->loc_hp[0] + victim->loc_hp[1] + victim->loc_hp[2] + 
	   victim->loc_hp[3] + victim->loc_hp[4] + victim->loc_hp[5] +
	   victim->loc_hp[6] ) == 0 )
    {
	act("$N has no apparent injuries.",ch,NULL,victim,TO_CHAR);
	send_to_char("--------------------------------------------------------------------------------\n\r",ch);
	return;
    }
    /* Check head */
    if (IS_HEAD(victim,LOST_EYE_L) && IS_HEAD(victim,LOST_EYE_R))
	act("$N has lost both of $S eyes.",ch,NULL,victim,TO_CHAR);
    else if (IS_HEAD(victim,LOST_EYE_L))
	act("$N has lost $S left eye.",ch,NULL,victim,TO_CHAR);
    else if (IS_HEAD(victim,LOST_EYE_R))
	act("$N has lost $S right eye.",ch,NULL,victim,TO_CHAR);
    if (IS_HEAD(victim,LOST_EAR_L) && IS_HEAD(victim,LOST_EAR_R))
	act("$N has lost both of $S ears.",ch,NULL,victim,TO_CHAR);
    else if (IS_HEAD(victim,LOST_EAR_L))
	act("$N has lost $S left ear.",ch,NULL,victim,TO_CHAR);
    else if (IS_HEAD(victim,LOST_EAR_R))
	act("$N has lost $S right ear.",ch,NULL,victim,TO_CHAR);
    if (IS_HEAD(victim,LOST_NOSE))
	act("$N has lost $S nose.",ch,NULL,victim,TO_CHAR);
    else if (IS_HEAD(victim,BROKEN_NOSE))
	act("$N has got a broken nose.",ch,NULL,victim,TO_CHAR);
    if (IS_HEAD(victim,BROKEN_JAW))
	act("$N has got a broken jaw.",ch,NULL,victim,TO_CHAR);
    if (IS_HEAD(victim,LOST_HEAD))
    {
	act("$N has had $S head cut off.",ch,NULL,victim,TO_CHAR);
	if (IS_BLEEDING(victim,BLEEDING_HEAD))
	    act("...Blood is spurting from the stump of $S neck.",ch,NULL,victim,TO_CHAR);
    }
    else
    {
	if (IS_BODY(victim,BROKEN_NECK))
	    act("$N has got a broken neck.",ch,NULL,victim,TO_CHAR);
	if (IS_BODY(victim,CUT_THROAT))
	{
	    act("$N has had $S throat cut open.",ch,NULL,victim,TO_CHAR);
	    if (IS_BLEEDING(victim,BLEEDING_THROAT))
		act("...Blood is pouring from the wound.",ch,NULL,victim,TO_CHAR);
	}
    }
    if (IS_HEAD(victim,BROKEN_SKULL))
	act("$N has got a broken skull.",ch,NULL,victim,TO_CHAR);
    if (IS_HEAD(victim,LOST_TOOTH_1 )) teeth += 1;
    if (IS_HEAD(victim,LOST_TOOTH_2 )) teeth += 2;
    if (IS_HEAD(victim,LOST_TOOTH_4 )) teeth += 4;
    if (IS_HEAD(victim,LOST_TOOTH_8 )) teeth += 8;
    if (IS_HEAD(victim,LOST_TOOTH_16)) teeth += 16;
    if (teeth > 0)
    {
	sprintf(buf,"$N has had %d teeth knocked out.",teeth);
	act(buf,ch,NULL,victim,TO_CHAR);
    }
    if (IS_HEAD(victim,LOST_TONGUE))
	act("$N has had $S tongue ripped out.",ch,NULL,victim,TO_CHAR);
    if (IS_HEAD(victim,LOST_HEAD))
    {
    	send_to_char("--------------------------------------------------------------------------------\n\r",ch);
    	return;
    }
    /* Check body */
    if (IS_BODY(victim,BROKEN_RIBS_1 )) ribs += 1;
    if (IS_BODY(victim,BROKEN_RIBS_2 )) ribs += 2;
    if (IS_BODY(victim,BROKEN_RIBS_4 )) ribs += 4;
    if (IS_BODY(victim,BROKEN_RIBS_8 )) ribs += 8;
    if (IS_BODY(victim,BROKEN_RIBS_16)) ribs += 16;
    if (ribs > 0)
    {
	sprintf(buf,"$N has got %d broken ribs.",ribs);
	act(buf,ch,NULL,victim,TO_CHAR);
    }
    if (IS_BODY(victim,BROKEN_SPINE))
	act("$N has got a broken spine.",ch,NULL,victim,TO_CHAR);
    /* Check arms */
    check_left_arm(ch,victim);
    check_right_arm(ch,victim);
    check_left_leg(ch,victim);
    check_right_leg(ch,victim);
    send_to_char("--------------------------------------------------------------------------------\n\r",ch);
    return;
}

void check_left_arm( CHAR_DATA *ch, CHAR_DATA *victim )
{
    char buf    [MAX_STRING_LENGTH];
    char finger [10];
    int fingers = 0;

    if (IS_ARM_L(victim,LOST_ARM) && IS_ARM_R(victim,LOST_ARM))
    {
	act("$N has lost both of $S arms.",ch,NULL,victim,TO_CHAR);
	if (IS_BLEEDING(victim,BLEEDING_ARM_L) && IS_BLEEDING(victim,BLEEDING_ARM_R))
	    act("...Blood is spurting from both stumps.",ch,NULL,victim,TO_CHAR);
	else if (IS_BLEEDING(victim,BLEEDING_ARM_L))
	    act("...Blood is spurting from the left stump.",ch,NULL,victim,TO_CHAR);
	else if (IS_BLEEDING(victim,BLEEDING_ARM_R))
	    act("...Blood is spurting from the right stump.",ch,NULL,victim,TO_CHAR);
	return;
    }
    if (IS_ARM_L(victim,LOST_ARM))
    {
	act("$N has lost $S left arm.",ch,NULL,victim,TO_CHAR);
	if (IS_BLEEDING(victim,BLEEDING_ARM_L))
	    act("...Blood is spurting from the stump.",ch,NULL,victim,TO_CHAR);
	return;
    }
    if (IS_ARM_L(victim,BROKEN_ARM) && IS_ARM_R(victim,BROKEN_ARM))
	act("$N arms are both broken.",ch,NULL,victim,TO_CHAR);
    else if (IS_ARM_L(victim,BROKEN_ARM))
	act("$N's left arm is broken.",ch,NULL,victim,TO_CHAR);
    if (IS_ARM_L(victim,LOST_HAND) && IS_ARM_R(victim,LOST_HAND) &&
	!IS_ARM_R(victim,LOST_ARM))
    {
	act("$N has lost both of $S hands.",ch,NULL,victim,TO_CHAR);
	if (IS_BLEEDING(victim,BLEEDING_HAND_L) && IS_BLEEDING(victim,BLEEDING_HAND_R))
	    act("...Blood is spurting from both stumps.",ch,NULL,victim,TO_CHAR);
	else if (IS_BLEEDING(victim,BLEEDING_HAND_L))
	    act("...Blood is spurting from the left stump.",ch,NULL,victim,TO_CHAR);
	else if (IS_BLEEDING(victim,BLEEDING_HAND_R))
	    act("...Blood is spurting from the right stump.",ch,NULL,victim,TO_CHAR);
	return;
    }
    if (IS_ARM_L(victim,LOST_HAND))
    {
	act("$N has lost $S left hand.",ch,NULL,victim,TO_CHAR);
	if (IS_BLEEDING(victim,BLEEDING_HAND_L))
	    act("...Blood is spurting from the stump.",ch,NULL,victim,TO_CHAR);
	return;
    }
    if (IS_ARM_L(victim,LOST_FINGER_I)) fingers += 1;
    if (IS_ARM_L(victim,LOST_FINGER_M)) fingers += 1;
    if (IS_ARM_L(victim,LOST_FINGER_R)) fingers += 1;
    if (IS_ARM_L(victim,LOST_FINGER_L)) fingers += 1;
    if (fingers == 1) sprintf(finger,"finger");
    else sprintf(finger,"fingers");
    if (fingers > 0 && IS_ARM_L(victim,LOST_THUMB))
    {
	sprintf(buf,"$N has lost %d %s and $S thumb from $S left hand.",fingers,finger);
	act(buf,ch,NULL,victim,TO_CHAR);
    }
    else if (fingers > 0)
    {
	sprintf(buf,"$N has lost %d %s from $S left hand.",fingers,finger);
	act(buf,ch,NULL,victim,TO_CHAR);
    }
    else if (IS_ARM_L(victim,LOST_THUMB))
    {
	sprintf(buf,"$N has lost the thumb from $S left hand.");
	act(buf,ch,NULL,victim,TO_CHAR);
    }
    fingers = 0;
    if (IS_ARM_L(victim,BROKEN_FINGER_I) && !IS_ARM_L(victim,LOST_FINGER_I)) fingers += 1;
    if (IS_ARM_L(victim,BROKEN_FINGER_M) && !IS_ARM_L(victim,LOST_FINGER_M)) fingers += 1;
    if (IS_ARM_L(victim,BROKEN_FINGER_R) && !IS_ARM_L(victim,LOST_FINGER_R)) fingers += 1;
    if (IS_ARM_L(victim,BROKEN_FINGER_L) && !IS_ARM_L(victim,LOST_FINGER_L)) fingers += 1;
    if (fingers == 1) sprintf(finger,"finger");
    else sprintf(finger,"fingers");
    if (fingers > 0 && IS_ARM_L(victim,BROKEN_THUMB) && !IS_ARM_L(victim,LOST_THUMB))
    {
	sprintf(buf,"$N has broken %d %s and $S thumb on $S left hand.",fingers,finger);
	act(buf,ch,NULL,victim,TO_CHAR);
    }
    else if (fingers > 0)
    {
	sprintf(buf,"$N has broken %d %s on $S left hand.",fingers,finger);
	act(buf,ch,NULL,victim,TO_CHAR);
    }
    else if (IS_ARM_L(victim,BROKEN_THUMB) && !IS_ARM_L(victim,LOST_THUMB))
    {
	sprintf(buf,"$N has broken the thumb on $S left hand.");
	act(buf,ch,NULL,victim,TO_CHAR);
    }
    return;
}

void check_right_arm( CHAR_DATA *ch, CHAR_DATA *victim )
{
    char buf    [MAX_STRING_LENGTH];
    char finger [10];
    int fingers = 0;

    if (IS_ARM_L(victim,LOST_ARM) && IS_ARM_R(victim,LOST_ARM))
	return;
    if (IS_ARM_R(victim,LOST_ARM))
    {
	act("$N has lost $S right arm.",ch,NULL,victim,TO_CHAR);
	if (IS_BLEEDING(victim,BLEEDING_ARM_R))
	    act("...Blood is spurting from the stump.",ch,NULL,victim,TO_CHAR);
	return;
    }
    if (!IS_ARM_L(victim,BROKEN_ARM) && IS_ARM_R(victim,BROKEN_ARM))
	act("$N's right arm is broken.",ch,NULL,victim,TO_CHAR);
    else if (IS_ARM_L(victim,LOST_ARM) && IS_ARM_R(victim,BROKEN_ARM))
	act("$N's right arm is broken.",ch,NULL,victim,TO_CHAR);
    if (IS_ARM_L(victim,LOST_HAND) && IS_ARM_R(victim,LOST_HAND))
	return;
    if (IS_ARM_R(victim,LOST_HAND))
    {
	act("$N has lost $S right hand.",ch,NULL,victim,TO_CHAR);
	if (IS_BLEEDING(victim,BLEEDING_HAND_R))
	    act("...Blood is spurting from the stump.",ch,NULL,victim,TO_CHAR);
	return;
    }
    if (IS_ARM_R(victim,LOST_FINGER_I)) fingers += 1;
    if (IS_ARM_R(victim,LOST_FINGER_M)) fingers += 1;
    if (IS_ARM_R(victim,LOST_FINGER_R)) fingers += 1;
    if (IS_ARM_R(victim,LOST_FINGER_L)) fingers += 1;
    if (fingers == 1) sprintf(finger,"finger");
    else sprintf(finger,"fingers");
    if (fingers > 0 && IS_ARM_R(victim,LOST_THUMB))
    {
	sprintf(buf,"$N has lost %d %s and $S thumb from $S right hand.",fingers,finger);
	act(buf,ch,NULL,victim,TO_CHAR);
    }
    else if (fingers > 0)
    {
	sprintf(buf,"$N has lost %d %s from $S right hand.",fingers,finger);
	act(buf,ch,NULL,victim,TO_CHAR);
    }
    else if (IS_ARM_R(victim,LOST_THUMB))
    {
	sprintf(buf,"$N has lost the thumb from $S right hand.");
	act(buf,ch,NULL,victim,TO_CHAR);
    }
    fingers = 0;
    if (IS_ARM_R(victim,BROKEN_FINGER_I) && !IS_ARM_R(victim,LOST_FINGER_I)) fingers += 1;
    if (IS_ARM_R(victim,BROKEN_FINGER_M) && !IS_ARM_R(victim,LOST_FINGER_M)) fingers += 1;
    if (IS_ARM_R(victim,BROKEN_FINGER_R) && !IS_ARM_R(victim,LOST_FINGER_R)) fingers += 1;
    if (IS_ARM_R(victim,BROKEN_FINGER_L) && !IS_ARM_R(victim,LOST_FINGER_L)) fingers += 1;
    if (fingers == 1) sprintf(finger,"finger");
    else sprintf(finger,"fingers");
    if (fingers > 0 && IS_ARM_R(victim,BROKEN_THUMB) && !IS_ARM_R(victim,LOST_THUMB))
    {
	sprintf(buf,"$N has broken %d %s and $S thumb on $S right hand.",fingers,finger);
	act(buf,ch,NULL,victim,TO_CHAR);
    }
    else if (fingers > 0)
    {
	sprintf(buf,"$N has broken %d %s on $S right hand.",fingers,finger);
	act(buf,ch,NULL,victim,TO_CHAR);
    }
    else if (IS_ARM_R(victim,BROKEN_THUMB) && !IS_ARM_R(victim,LOST_THUMB))
    {
	sprintf(buf,"$N has broken the thumb on $S right hand.");
	act(buf,ch,NULL,victim,TO_CHAR);
    }
    return;
}

void check_left_leg( CHAR_DATA *ch, CHAR_DATA *victim )
{
    char buf    [MAX_STRING_LENGTH];
    char toe [10];
    int toes = 0;

    if (IS_LEG_L(victim,LOST_LEG) && IS_LEG_R(victim,LOST_LEG))
    {
	act("$N has lost both of $S legs.",ch,NULL,victim,TO_CHAR);
	if (IS_BLEEDING(victim,BLEEDING_LEG_L) && IS_BLEEDING(victim,BLEEDING_LEG_R))
	    act("...Blood is spurting from both stumps.",ch,NULL,victim,TO_CHAR);
	else if (IS_BLEEDING(victim,BLEEDING_LEG_L))
	    act("...Blood is spurting from the left stump.",ch,NULL,victim,TO_CHAR);
	else if (IS_BLEEDING(victim,BLEEDING_LEG_R))
	    act("...Blood is spurting from the right stump.",ch,NULL,victim,TO_CHAR);
	return;
    }
    if (IS_LEG_L(victim,LOST_LEG))
    {
	act("$N has lost $S left leg.",ch,NULL,victim,TO_CHAR);
	if (IS_BLEEDING(victim,BLEEDING_LEG_L))
	    act("...Blood is spurting from the stump.",ch,NULL,victim,TO_CHAR);
	return;
    }
    if (IS_LEG_L(victim,BROKEN_LEG) && IS_LEG_R(victim,BROKEN_LEG))
	act("$N legs are both broken.",ch,NULL,victim,TO_CHAR);
    else if (IS_LEG_L(victim,BROKEN_LEG))
	act("$N's left leg is broken.",ch,NULL,victim,TO_CHAR);
    if (IS_LEG_L(victim,LOST_FOOT) && IS_LEG_R(victim,LOST_FOOT))
    {
	act("$N has lost both of $S feet.",ch,NULL,victim,TO_CHAR);
	if (IS_BLEEDING(victim,BLEEDING_FOOT_L) && IS_BLEEDING(victim,BLEEDING_FOOT_R))
	    act("...Blood is spurting from both stumps.",ch,NULL,victim,TO_CHAR);
	else if (IS_BLEEDING(victim,BLEEDING_FOOT_L))
	    act("...Blood is spurting from the left stump.",ch,NULL,victim,TO_CHAR);
	else if (IS_BLEEDING(victim,BLEEDING_FOOT_R))
	    act("...Blood is spurting from the right stump.",ch,NULL,victim,TO_CHAR);
	return;
    }
    if (IS_LEG_L(victim,LOST_FOOT))
    {
	act("$N has lost $S left foot.",ch,NULL,victim,TO_CHAR);
	if (IS_BLEEDING(victim,BLEEDING_FOOT_L))
	    act("...Blood is spurting from the stump.",ch,NULL,victim,TO_CHAR);
	return;
    }
    if (IS_LEG_L(victim,LOST_TOE_A)) toes += 1;
    if (IS_LEG_L(victim,LOST_TOE_B)) toes += 1;
    if (IS_LEG_L(victim,LOST_TOE_C)) toes += 1;
    if (IS_LEG_L(victim,LOST_TOE_D)) toes += 1;
    if (toes == 1) sprintf(toe,"toe");
    else sprintf(toe,"toes");
    if (toes > 0 && IS_LEG_L(victim,LOST_TOE_BIG))
    {
	sprintf(buf,"$N has lost %d %s and $S big toe from $S left foot.",toes,toe);
	act(buf,ch,NULL,victim,TO_CHAR);
    }
    else if (toes > 0)
    {
	sprintf(buf,"$N has lost %d %s from $S left foot.",toes,toe);
	act(buf,ch,NULL,victim,TO_CHAR);
    }
    else if (IS_LEG_L(victim,LOST_TOE_BIG))
    {
	sprintf(buf,"$N has lost the big toe from $S left foot.");
	act(buf,ch,NULL,victim,TO_CHAR);
    }
    toes = 0;
    if (IS_LEG_L(victim,BROKEN_TOE_A) && !IS_LEG_L(victim,LOST_TOE_A)) toes += 1;
    if (IS_LEG_L(victim,BROKEN_TOE_B) && !IS_LEG_L(victim,LOST_TOE_B)) toes += 1;
    if (IS_LEG_L(victim,BROKEN_TOE_C) && !IS_LEG_L(victim,LOST_TOE_C)) toes += 1;
    if (IS_LEG_L(victim,BROKEN_TOE_D) && !IS_LEG_L(victim,LOST_TOE_D)) toes += 1;
    if (toes == 1) sprintf(toe,"toe");
    else sprintf(toe,"toes");
    if (toes > 0 && IS_LEG_L(victim,BROKEN_TOE_BIG) && !IS_LEG_L(victim,LOST_TOE_BIG))
    {
	sprintf(buf,"$N has broken %d %s and $S big toe from $S left foot.",toes,toe);
	act(buf,ch,NULL,victim,TO_CHAR);
    }
    else if (toes > 0)
    {
	sprintf(buf,"$N has broken %d %s on $S left foot.",toes,toe);
	act(buf,ch,NULL,victim,TO_CHAR);
    }
    else if (IS_LEG_L(victim,BROKEN_TOE_BIG) && !IS_LEG_L(victim,LOST_TOE_BIG))
    {
	sprintf(buf,"$N has broken the big toe on $S left foot.");
	act(buf,ch,NULL,victim,TO_CHAR);
    }
    return;
}

void check_right_leg( CHAR_DATA *ch, CHAR_DATA *victim )
{
    char buf    [MAX_STRING_LENGTH];
    char toe [10];
    int toes = 0;

    if (IS_LEG_L(victim,LOST_LEG) && IS_LEG_R(victim,LOST_LEG))
	return;
    if (IS_LEG_R(victim,LOST_LEG))
    {
	act("$N has lost $S right leg.",ch,NULL,victim,TO_CHAR);
	if (IS_BLEEDING(victim,BLEEDING_LEG_R))
	    act("...Blood is spurting from the stump.",ch,NULL,victim,TO_CHAR);
	return;
    }
    if (!IS_LEG_L(victim,BROKEN_LEG) && IS_LEG_R(victim,BROKEN_LEG))
	act("$N's right leg is broken.",ch,NULL,victim,TO_CHAR);
    if (IS_LEG_L(victim,LOST_FOOT) && IS_LEG_R(victim,LOST_FOOT))
	return;
    if (IS_LEG_R(victim,LOST_FOOT))
    {
	act("$N has lost $S right foot.",ch,NULL,victim,TO_CHAR);
	if (IS_BLEEDING(victim,BLEEDING_FOOT_R))
	    act("...Blood is spurting from the stump.",ch,NULL,victim,TO_CHAR);
	return;
    }
    if (IS_LEG_R(victim,LOST_TOE_A)) toes += 1;
    if (IS_LEG_R(victim,LOST_TOE_B)) toes += 1;
    if (IS_LEG_R(victim,LOST_TOE_C)) toes += 1;
    if (IS_LEG_R(victim,LOST_TOE_D)) toes += 1;
    if (toes == 1) sprintf(toe,"toe");
    else sprintf(toe,"toes");
    if (toes > 0 && IS_LEG_R(victim,LOST_TOE_BIG))
    {
	sprintf(buf,"$N has lost %d %s and $S big toe from $S right foot.",toes,toe);
	act(buf,ch,NULL,victim,TO_CHAR);
    }
    else if (toes > 0)
    {
	sprintf(buf,"$N has lost %d %s from $S right foot.",toes,toe);
	act(buf,ch,NULL,victim,TO_CHAR);
    }
    else if (IS_LEG_R(victim,LOST_TOE_BIG))
    {
	sprintf(buf,"$N has lost the big toe from $S right foot.");
	act(buf,ch,NULL,victim,TO_CHAR);
    }
    toes = 0;
    if (IS_LEG_R(victim,BROKEN_TOE_A) && !IS_LEG_R(victim,LOST_TOE_A)) toes += 1;
    if (IS_LEG_R(victim,BROKEN_TOE_B) && !IS_LEG_R(victim,LOST_TOE_B)) toes += 1;
    if (IS_LEG_R(victim,BROKEN_TOE_C) && !IS_LEG_R(victim,LOST_TOE_C)) toes += 1;
    if (IS_LEG_R(victim,BROKEN_TOE_D) && !IS_LEG_R(victim,LOST_TOE_D)) toes += 1;
    if (toes == 1) sprintf(toe,"toe");
    else sprintf(toe,"toes");
    if (toes > 0 && IS_LEG_R(victim,BROKEN_TOE_BIG) && !IS_LEG_R(victim,LOST_TOE_BIG))
    {
	sprintf(buf,"$N has broken %d %s and $S big toe on $S right foot.",toes,toe);
	act(buf,ch,NULL,victim,TO_CHAR);
    }
    else if (toes > 0)
    {
	sprintf(buf,"$N has broken %d %s on $S right foot.",toes,toe);
	act(buf,ch,NULL,victim,TO_CHAR);
    }
    else if (IS_LEG_R(victim,BROKEN_TOE_BIG) && !IS_LEG_R(victim,LOST_TOE_BIG))
    {
	sprintf(buf,"$N has broken the big toe on $S right foot.");
	act(buf,ch,NULL,victim,TO_CHAR);
    }
    return;
}

void obj_score( CHAR_DATA *ch, OBJ_DATA *obj )
{
    char buf[MAX_STRING_LENGTH];
    AFFECT_DATA *paf;
    int itemtype;

    sprintf( buf,"You are %s.\n\r",obj->short_descr);
    send_to_char( buf, ch );

    sprintf( buf,"Type %s, Extra flags %s.\n\r",item_type_name(obj->item_type),
	extra_bit_name(obj->extra_flags));
    send_to_char( buf, ch );

    sprintf( buf,"You weigh %d pounds and are worth %d gold coins.\n\r",obj->weight,obj->cost);
    send_to_char( buf, ch );

    if (obj->questmaker != NULL && strlen(obj->questmaker) > 1 &&
	obj->questowner != NULL && strlen(obj->questowner) > 1)
    {
	sprintf( buf, "You were created by %s, and are owned by %s.\n\r", obj->questmaker,obj->questowner );
	send_to_char( buf, ch );
    }
    else if (obj->questmaker != NULL && strlen(obj->questmaker) > 1)
    {
	sprintf( buf, "You were created by %s.\n\r", obj->questmaker );
	send_to_char( buf, ch );
    }
    else if (obj->questowner != NULL && strlen(obj->questowner) > 1)
    {
	sprintf( buf, "You are owned by %s.\n\r", obj->questowner );
	send_to_char( buf, ch );
    }

    switch ( obj->item_type )
    {
    case ITEM_SCROLL: 
    case ITEM_POTION:
	sprintf( buf, "You contain level %d spells of:", obj->value[0] );
	send_to_char( buf, ch );

	if ( obj->value[1] >= 0 && obj->value[1] < MAX_SKILL )
	{
	    send_to_char( " '", ch );
	    send_to_char( skill_table[obj->value[1]].name, ch );
	    send_to_char( "'", ch );
	}

	if ( obj->value[2] >= 0 && obj->value[2] < MAX_SKILL )
	{
	    send_to_char( " '", ch );
	    send_to_char( skill_table[obj->value[2]].name, ch );
	    send_to_char( "'", ch );
	}

	if ( obj->value[3] >= 0 && obj->value[3] < MAX_SKILL )
	{
	    send_to_char( " '", ch );
	    send_to_char( skill_table[obj->value[3]].name, ch );
	    send_to_char( "'", ch );
	}

	send_to_char( ".\n\r", ch );
	break;

    case ITEM_QUEST:
	sprintf( buf, "Your quest point value is %d.\n\r", obj->value[0] );
	send_to_char( buf, ch );
	break;

    case ITEM_WAND: 
    case ITEM_STAFF: 
	sprintf( buf, "You have %d(%d) charges of level %d",
	    obj->value[1], obj->value[2], obj->value[0] );
	send_to_char( buf, ch );

	if ( obj->value[3] >= 0 && obj->value[3] < MAX_SKILL )
	{
	    send_to_char( " '", ch );
	    send_to_char( skill_table[obj->value[3]].name, ch );
	    send_to_char( "'", ch );
	}

	send_to_char( ".\n\r", ch );
	break;
      
    case ITEM_WEAPON:
	sprintf( buf, "You inflict %d to %d damage in combat (average %d).\n\r",
	    obj->value[1], obj->value[2],
	    ( obj->value[1] + obj->value[2] ) / 2 );
	send_to_char( buf, ch );

	if (obj->value[0] >= 1000)
	    itemtype = obj->value[0] - ((obj->value[0] / 1000) * 1000);
	else
	    itemtype = obj->value[0];

	if (itemtype > 0)
	{
	    if (obj->level < 10)
		sprintf(buf,"You are a minor spell weapon.\n\r");
	    else if (obj->level < 20)
		sprintf(buf,"You are a lesser spell weapon.\n\r");
	    else if (obj->level < 30)
		sprintf(buf,"You are an average spell weapon.\n\r");
	    else if (obj->level < 40)
		sprintf(buf,"You are a greater spell weapon.\n\r");
	    else if (obj->level < 50)
		sprintf(buf,"You are a major spell weapon.\n\r");
	    else
		sprintf(buf,"You are a supreme spell weapon.\n\r");
	    send_to_char(buf,ch);
	}

	if (itemtype == 1)
	    sprintf (buf, "You are dripping with corrosive acid.\n\r");
	else if (itemtype == 4)
	    sprintf (buf, "You radiate an aura of darkness.\n\r");
	else if (itemtype == 30)
	    sprintf (buf, "You are the bane of all evil.\n\r");
	else if (itemtype == 34)
	    sprintf (buf, "You drink the souls of your victims.\n\r");
	else if (itemtype == 37)
	    sprintf (buf, "You have been tempered in hellfire.\n\r");
	else if (itemtype == 48)
	    sprintf (buf, "You crackle with sparks of lightning.\n\r");
	else if (itemtype == 53)
	    sprintf (buf, "You are dripping with a dark poison.\n\r");
	else if (itemtype > 0)
	    sprintf (buf, "You have been imbued with the power of %s.\n\r",skill_table[itemtype].name);
	if (itemtype > 0)
	    send_to_char( buf, ch );

	if (obj->value[0] >= 1000)
	    itemtype = obj->value[0] / 1000;
	else
	    break;

	if (itemtype == 4 || itemtype == 1)
	    sprintf (buf, "You radiate an aura of darkness.\n\r");
	else if (itemtype == 27 || itemtype == 2)
	    sprintf (buf, "You allow your wielder to see invisible things.\n\r");
	else if (itemtype == 39 || itemtype == 3)
	    sprintf (buf, "You grant your wielder the power of flight.\n\r");
	else if (itemtype == 45 || itemtype == 4)
	    sprintf (buf, "You allow your wielder to see in the dark.\n\r");
	else if (itemtype == 46 || itemtype == 5)
	    sprintf (buf, "You render your wielder invisible to the human eye.\n\r");
	else if (itemtype == 52 || itemtype == 6)
	    sprintf (buf, "You allow your wielder to walk through solid doors.\n\r");
	else if (itemtype == 54 || itemtype == 7)
	    sprintf (buf, "You protect your wielder from evil.\n\r");
	else if (itemtype == 57 || itemtype == 8)
	    sprintf (buf, "You protect your wielder in combat.\n\r");
	else if (itemtype == 9)
	    sprintf (buf, "You allow your wielder to walk in complete silence.\n\r");
	else if (itemtype == 10)
	    sprintf (buf, "You surround your wielder with a shield of lightning.\n\r");
	else if (itemtype == 11)
	    sprintf (buf, "You surround your wielder with a shield of fire.\n\r");
	else if (itemtype == 12)
	    sprintf (buf, "You surround your wielder with a shield of ice.\n\r");
	else if (itemtype == 13)
	    sprintf (buf, "You surround your wielder with a shield of acid.\n\r");
	else if (itemtype == 14)
	    sprintf (buf, "You protect your wielder from attacks from DarkBlade clan guardians.\n\r");
	else if (itemtype == 15)
	    sprintf (buf, "You surround your wielder with a shield of chaos.\n\r");
	else if (itemtype == 16)
	    sprintf (buf, "You regenerate the wounds of your wielder.\n\r");
	else if (itemtype == 17)
	    sprintf (buf, "You enable your wielder to move at supernatural speed.\n\r");
	else if (itemtype == 18)
	    sprintf (buf, "You can slice through armour without difficulty.\n\r");
	else if (itemtype == 19)
	    sprintf (buf, "You protect your wielder from player attacks.\n\r");
	else if (itemtype == 20)
	    sprintf (buf, "You surround your wielder with a shield of darkness.\n\r");
	else if (itemtype == 21)
	    sprintf (buf, "You grant your wielder superior protection.\n\r");
	else if (itemtype == 22)
	    sprintf (buf, "You grant your wielder supernatural vision.\n\r");
	else if (itemtype == 23)
	    sprintf (buf, "You make your wielder fleet-footed.\n\r");
	else if (itemtype == 24)
	    sprintf (buf, "You conceal your wielder from sight.\n\r");
	else if (itemtype == 25)
	    sprintf (buf, "You invoke the power of your wielders beast.\n\r");
	else
	    sprintf (buf, "You are bugged...please report it.\n\r");
	if (itemtype > 0)
	    send_to_char( buf, ch );
	break;

    case ITEM_ARMOR:
	sprintf( buf, "Your armor class is %d.\n\r", obj->value[0] );
	send_to_char( buf, ch );
	if (obj->value[3] < 1)
	    break;
	if (obj->value[3] == 4 || obj->value[3] == 1)
	    sprintf (buf, "You radiate an aura of darkness.\n\r");
	else if (obj->value[3] == 27 || obj->value[3] == 2)
	    sprintf (buf, "You allow your wearer to see invisible things.\n\r");
	else if (obj->value[3] == 39 || obj->value[3] == 3)
	    sprintf (buf, "You grant your wearer the power of flight.\n\r");
	else if (obj->value[3] == 45 || obj->value[3] == 4)
	    sprintf (buf, "You allow your wearer to see in the dark.\n\r");
	else if (obj->value[3] == 46 || obj->value[3] == 5)
	    sprintf (buf, "You render your wearer invisible to the human eye.\n\r");
	else if (obj->value[3] == 52 || obj->value[3] == 6)
	    sprintf (buf, "You allow your wearer to walk through solid doors.\n\r");
	else if (obj->value[3] == 54 || obj->value[3] == 7)
	    sprintf (buf, "You protect your wearer from evil.\n\r");
	else if (obj->value[3] == 57 || obj->value[3] == 8)
	    sprintf (buf, "You protect your wearer in combat.\n\r");
	else if (obj->value[3] == 9)
	    sprintf (buf, "You allow your wearer to walk in complete silence.\n\r");
	else if (obj->value[3] == 10)
	    sprintf (buf, "You surround your wearer with a shield of lightning.\n\r");
	else if (obj->value[3] == 11)
	    sprintf (buf, "You surround your wearer with a shield of fire.\n\r");
	else if (obj->value[3] == 12)
	    sprintf (buf, "You surround your wearer with a shield of ice.\n\r");
	else if (obj->value[3] == 13)
	    sprintf (buf, "You surround your wearer with a shield of acid.\n\r");
	else if (obj->value[3] == 14)
	    sprintf (buf, "You protect your wearer from attacks from DarkBlade clan guardians.\n\r");
	else if (obj->value[3] == 15)
	    sprintf (buf, "You surround your wielder with a shield of chaos.\n\r");
	else if (obj->value[3] == 16)
	    sprintf (buf, "You regenerate the wounds of your wielder.\n\r");
	else if (obj->value[3] == 17)
	    sprintf (buf, "You enable your wearer to move at supernatural speed.\n\r");
	else if (obj->value[3] == 18)
	    sprintf (buf, "You can slice through armour without difficulty.\n\r");
	else if (obj->value[3] == 19)
	    sprintf (buf, "You protect your wearer from player attacks.\n\r");
	else if (obj->value[3] == 20)
	    sprintf (buf, "You surround your wearer with a shield of darkness.\n\r");
	else if (obj->value[3] == 21)
	    sprintf (buf, "You grant your wearer superior protection.\n\r");
	else if (obj->value[3] == 22)
	    sprintf (buf, "You grant your wearer supernatural vision.\n\r");
	else if (obj->value[3] == 23)
	    sprintf (buf, "You make your wearer fleet-footed.\n\r");
	else if (obj->value[3] == 24)
	    sprintf (buf, "You conceal your wearer from sight.\n\r");
	else if (obj->value[3] == 25)
	    sprintf (buf, "You invoke the power of your wearers beast.\n\r");
	else
	    sprintf (buf, "You are bugged...please report it.\n\r");
	if (obj->value[3] > 0)
	    send_to_char( buf, ch );
	break;
    }

    for ( paf = obj->pIndexData->affected; paf != NULL; paf = paf->next )
    {
	if ( paf->location != APPLY_NONE && paf->modifier != 0 )
	{
	    sprintf( buf, "You affect %s by %d.\n\r",
		affect_loc_name( paf->location ), paf->modifier );
	    send_to_char( buf, ch );
	}
    }

    for ( paf = obj->affected; paf != NULL; paf = paf->next )
    {
	if ( paf->location != APPLY_NONE && paf->modifier != 0 )
	{
	    sprintf( buf, "You affect %s by %d.\n\r",
		affect_loc_name( paf->location ), paf->modifier );
	    send_to_char( buf, ch );
	}
    }
    return;
}

/* Do_prompt from Morgenes from Aldara Mud */
void do_prompt( CHAR_DATA *ch, char *argument )
{
   char buf [ MAX_STRING_LENGTH ];

   buf[0] = '\0';
   if ( IS_NPC(ch) ) return;

   if ( argument[0] == '\0' )
   {
       do_help( ch, "prompt" );
       return;
   }

   if( !strcmp( argument, "on" ) )
   {
      if (IS_EXTRA(ch, EXTRA_PROMPT))
         send_to_char("But you already have customised prompt on!\n\r",ch);
      else
      {
         send_to_char("Ok.\n\r",ch);
         SET_BIT(ch->extra, EXTRA_PROMPT);
      }
      return;
   }
   else if( !strcmp( argument, "off" ) )
   {
      if (!IS_EXTRA(ch, EXTRA_PROMPT))
         send_to_char("But you already have customised prompt off!\n\r",ch);
      else
      {
         send_to_char("Ok.\n\r",ch);
         REMOVE_BIT(ch->extra, EXTRA_PROMPT);
      }
      return;
   }
   else if( !strcmp( argument, "clear" ) )
   {
      free_string(ch->prompt);
      ch->prompt = str_dup( "" );
      return;
   }
   else
   {
      if ( strlen( argument ) > 50 )
	  argument[50] = '\0';
      smash_tilde( argument );
      strcat( buf, argument );
   }

   free_string( ch->prompt );
   ch->prompt = str_dup( buf );
   send_to_char( "Ok.\n\r", ch );
   return;
} 

/* Do_prompt from Morgenes from Aldara Mud */
void do_cprompt( CHAR_DATA *ch, char *argument )
{
   char buf [ MAX_STRING_LENGTH ];

   buf[0] = '\0';
   if ( IS_NPC(ch) ) return;

   if ( argument[0] == '\0' )
   {
       do_help( ch, "cprompt" );
       return;
   }

   if( !strcmp( argument, "clear" ) )
   {
      free_string(ch->cprompt);
      ch->cprompt = str_dup( "" );
      return;
   }
   else
   {
      if ( strlen( argument ) > 50 )
	  argument[50] = '\0';
      smash_tilde( argument );
      strcat( buf, argument );
   }

   free_string( ch->cprompt );
   ch->cprompt = str_dup( buf );
   send_to_char( "Ok.\n\r", ch );
   return;
} 

void do_imbue( CHAR_DATA *ch, char *argument )
{
  OBJ_DATA *obj;
  AFFECT_DATA *paf;
  char arg[MIL];
   
  argument = one_argument( argument, arg );
  
  if ( IS_NPC(ch) ) return;
       
  if ( ch->lstatus < 5 )
  {
    stc("You must have at least 5 legend status.\n\r",ch);
    return;
  }
   
  if ( arg[0] == '\0' )
  {
    stc("Which piece of armor do you want to imbue?\n\r",ch);
    return;
  }
   
  if ( ( obj = get_obj_carry( ch, arg ) ) == NULL )
  {
    stc("You are not carrying that object.\n\r",ch);
    return;
  }
   
  if( IS_SET(obj->quest, QUEST_MASTERY) )
  {
    stc("This object can not be affected by your magic.\n\r",ch);
    return;
  } 
   
  if ( IS_SET(obj->quest, QUEST_IMBUE) )
  {
    stc("This object has already been imbued.\n\r",ch);
    return;
  }   
   
  if ( obj->item_type != ITEM_ARMOR )
  {
    stc("This item isn't a piece of armor.\n\r",ch);
    return;
  }
   
  if ( ch->pcdata->quest < 1500 )
  {
    stc("You need 1500 QP to imbue this item with magic.\n\r",ch);
    return;
  }
   
  ch->pcdata->quest -= 1500;

  if (number_range(1,2) == 1)
  { 
      act("You attempt to chant some legendary words on $p.\n$p explodes into tiny pieces!",ch,obj,NULL,TO_CHAR);
    obj_from_char(obj);
    extract_obj(obj);
    return;
   }

  act("You chant some legendary words. $p begins to glow brightly.",ch,obj,NULL,TO_CHAR);
  act("$n's chants some legendary words.\n$p begins to glow brightly.\n",ch,obj,NULL,TO_ROOM);
  SET_BIT(obj->quest, QUEST_IMBUE);
   
  
  if ( affect_free == NULL )
  {
      paf             = alloc_perm( sizeof(*paf) );
  }   
  else
  {
      paf             = affect_free;
      affect_free     = affect_free->next;
  }
   
  paf->type           = 0;
  paf->duration       = -1;   
  paf->location       = APPLY_HITROLL;
  paf->modifier       = 30;
  paf->bitvector      = 0;
  paf->next           = obj->affected;
  obj->affected       = paf;
  
  if ( affect_free == NULL )
  {
      paf             = alloc_perm( sizeof(*paf) );
  }
  else
  {
      paf             = affect_free;
      affect_free     = affect_free->next;
  }   
   
  paf->type           = 0;
  paf->duration       = -1;
  paf->location       = APPLY_DAMROLL;
  paf->modifier       = 30;
  paf->bitvector      = 0;
  paf->next           = obj->affected;
  obj->affected       = paf;
 
  return;
} 
