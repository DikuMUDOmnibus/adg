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


void do_study( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
         if (ch->pcdata->powerst[20] == 2)
         {
             stc("-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n\r",ch);
             if (ch->pcdata->powerst[0] == 1)
             {
               sprintf(buf," Lvl:  1      enhanced damage    0 Mana    1 Prac [====================] %d\n\r",ch->pcdata->powerst[3]);
               stc(buf,ch);
             }
             stc("-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n\r",ch);
          }
     } 

 return;
}

void do_trait( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    int skill;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
stc("{D-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-{x\n\r",ch);
stc("{W                           Traits{x\n\r",ch);
stc("{D-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-{x\n\r",ch);
if (ch->pcdata->powerst[20] == 0) {
stc("{w You don't have a primary trait, type '{Rtrait <magician/thief/warrior/cleric>{w'\n\r",ch);
stc("{w to set your primary trait.\n\r",ch);
}
else if (ch->pcdata->powerst[20] == 1) {
sprintf(buf,"{B       Magician {D- {wLevel {D[{m%3d{D]{w Skill Points {D[{m%4d{D]{x\n\r",ch->pcdata->powerst[0],ch->pcdata->powerst[1]);
stc(buf,ch); }
else if (ch->pcdata->powerst[20] == 2) {
sprintf(buf,"{D       Thief    {D- {wLevel {D[{m%3d{D]{w Skill Points {D[{m%4d{D]{x\n\r",ch->pcdata->powerst[0],ch->pcdata->powerst[1]);
stc(buf,ch); }
else if (ch->pcdata->powerst[20] == 3) {
sprintf(buf,"{R       Warrior  {D- {wLevel {D[{m%3d{D]{w Skill Points {D[{m%4d{D]{x\n\r",ch->pcdata->powerst[0],ch->pcdata->powerst[1]);
stc(buf,ch);}
else if (ch->pcdata->powerst[20] == 4) {
sprintf(buf,"{C       Cleric   {D- {wLevel {D[{m%3d{D]{w Skill Points {D[{m%4d{D]{x\n\r",ch->pcdata->powerst[0],ch->pcdata->powerst[1]);
stc(buf,ch);}
stc("{D-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-{x\n\r",ch);
stc("{w To gain a level in your trait type: {Rtrait gain{x\n\r",ch);
stc("{w Cost: {Rtrait level * 10000000{x\n\r",ch);
stc("{D-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-{x\n\r",ch);
return;
}

if (!str_cmp(arg,"reset"))
{
   ch->pcdata->powerst[20] = 0;
   stc("Ok.\n\r",ch);
   return;
}

if (!str_cmp(arg,"gain"))
{
      if (ch->exp < ch->pcdata->powerst[0] * 10000000)
      {
         stc("You don't have enough exp.\n\r",ch); return;
      }
      else
      {   
         ch->exp -= ch->pcdata->powerst[0] * 10000000;
         ch->pcdata->powerst[0] += 1;
         stc("You gain a trait level!\n\r",ch);
         skill = number_range(10,20);
         ch->pcdata->powerst[1] += skill;
         sprintf(buf,"You recieve %d skill points.\n\r",skill);                      
         stc(buf,ch);
      }
   }   

if (!str_cmp(arg,"magician") && ch->pcdata->powerst[20] == 0) { ch->pcdata->powerst[20] = 1; stc("You have set your primary trait to Magician.\n\r",ch); }
else if (!str_cmp(arg,"thief") && ch->pcdata->powerst[20] == 0){ ch->pcdata->powerst[20] = 2; stc("You have set your primary trait to Thief.\n\r",ch); }
else if (!str_cmp(arg,"warrior") && ch->pcdata->powerst[20] == 0){ ch->pcdata->powerst[20] = 3; stc("You have set your primary trait to Warrior.\n\r",ch); }
else if (!str_cmp(arg,"cleric") && ch->pcdata->powerst[20] == 0){ ch->pcdata->powerst[20] = 4; stc("You have set your primary trait to Cleric.\n\r",ch); }
else if (!str_cmp(arg,"none") && ch->pcdata->powerst[20] == 0){ ch->pcdata->powerst[20] = 0; stc("You have cleared your primary trait.\n\r",ch); }
else { stc("That is not a valid trait!\n\r",ch); return; }

return;
}
