/******************************************************************************
 *
 *	File:			wpndefs.h
 *
 *	Function:		Declarations for wpndefs.h
 *
 *	Author(s):		Queen Serenity of the Moon Kingdom
 *
 *	Copyright:		Copyright (c) 1999 Queen Serenity of the Moon Kingdom
 *					All Rights Reserved.
 *
 *	Notes:			This header file stores all the weapon types currently used
 *                  in the format of WPN_<type> for the weapon number.
 *                  and PLR_<type> for the ch->wpn[number]
 *
 *                  WARNING don't try and use WPN_NONE to grab information.
 *                  as it will grab information from the array BEFORE ch->wpn.
 *
 ******************************************************************************/
/***************************************************************************
 *  WotK public release                                                    *
 *  (c)2000 'Queen Serenity' and 'Tamas D'Varde (Ryan Pessa)'              *
 *  Be sure to read WotK.doc in the docs directory.                        *
 *  Have fun! :)                                                           *
 ***************************************************************************/

#define WPN_NONE		-1
#define WPN_UNARMED		0
#define WPN_SLICE		1
#define WPN_STAB		2
#define WPN_SLASH		3
#define WPN_WHIP		4
#define WPN_CLAW		5
#define WPN_BLAST		6
#define WPN_POUND		7
#define WPN_CRUSH		8
#define WPN_GREP		9
#define WPN_BITE		10
#define WPN_PIERCE		11
#define WPN_SUCK		12
#define PLR_UNARMED(c)	(c)->wpn[0]
#define PLR_SLICE(c)	(c)->wpn[1]
#define PLR_STAB(c)		(c)->wpn[2]
#define PLR_SLASH(c)	(c)->wpn[3]
#define PLR_WHIP(c)		(c)->wpn[4]
#define PLR_CLAW(c)		(c)->wpn[5]
#define PLR_BLAST(c)	(c)->wpn[6]
#define PLR_POUND(c)	(c)->wpn[7]
#define PLR_CRUSH(c)	(c)->wpn[8]
#define PLR_GREP(c)		(c)->wpn[9]
#define PLR_BITE(c)		(c)->wpn[10]
#define PLR_PIERCE(c)	(c)->wpn[11]
#define PLR_SUCK(c)		(c)->wpn[12]
