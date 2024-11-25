#ifndef TYPETABLE_H
#define TYPETABLE_H
#include <iostream>
#include <map>

std::map<std::string, int> typesMap{{"normal", 0}, {"fire",  1}, {"water",  2}, {"electrik", 3}, {"grass",   4},
                                    {"ice",    5}, {"fight", 6}, {"poison", 7}, {"ground",   8}, {"flying",  9},
                                    {"psy",   10}, {"bug",  11}, {"rock",  12}, {"ghost",   13}, {"dragon", 14}};

float typesChart[15][15] = {
//               Normal Fire   Water  Elec   Grass  Ice    Fight  Poison Ground Flying Psy    Bug    Rock   Ghost  Dragon
/* Normal */   { 1,     1,     1,     1,     1,     1,     1,     1,     1,     1,     1,     1,     0.5,   0,     1   },
/* Fire */     { 1,     0.5,   0.5,   1,     2,     2,     1,     1,     1,     1,     1,     2,     0.5,   1,     0.5 },
/* Water */    { 1,     2,     0.5,   1,     0.5,   1,     1,     1,     2,     1,     1,     1,     2,     1,     0.5 },
/* Electric */ { 1,     1,     2,     0.5,   0.5,   1,     1,     1,     0,     2,     1,     1,     1,     1,     0.5 },
/* Grass */    { 1,     0.5,   2,     1,     0.5,   1 ,    1,     0.5,   2,     0.5,   1,     0.5,   2,     1,     0.5 },
/* Ice */      { 1,     0.5,   0.5,   1,     2,     0.5,   1,     1,     2,     2,     1,     1,     1,     1,     2   },
/* Fighting */ { 2,     1,     1,     1,     1,     2,     1,     0.5,   1,     0.5,   0.5,   0.5,   2,     0,     1   },
/* Poison */   { 1,     1,     1,     1,     2,     1,     1,     0.5,   0.5,   1,     1,     2,     0.5,   0.5,   1   },
/* Ground */   { 1,     2,     1,     2,     0.5,   1,     1,     2,     1,     0,     1,     0.5,   2,     1,     1   },
/* Flying */   { 1,     1,     1,     0.5,   2,     1,     2,     1,     1,     1,     1,     2,     0.5,   1,     1   },
/* Psychic */  { 1,     1,     1,     1,     1,     1,     2,     2,     1,     1,     0.5,   1,     1,     1,     1   },
/* Bug */      { 1,     0.5,   1,     1,     2,     1,     0.5,   2,     1,     0.5,   2,     1,     1,     0.5,   1   },
/* Rock */     { 1,     2,     1,     1,     1,     2,     0.5,   1,     0.5,   2,     1,     2,     1,     1,     1   },
/* Ghost */    { 1,     1,     1,     1,     1,     1,     1,     1,     1,     1,     0,     1,     1,     2,     1   },
/* Dragon */   { 1,     1,     1,     1,     1,     1,     1,     1,     1,     1,     1,     1,     1,     1,     2   }
};

#endif // TYPETABLE_H
