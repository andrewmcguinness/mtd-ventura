#pragma once

#include <iostream>
#include "game.h"

using std::ostream;

ostream& operator << (ostream& o, tile);
ostream& operator << (ostream& o, const std::vector<tile>&);
ostream& operator << (ostream& o, const track& tr);
ostream& operator << (ostream& o, const board& bd);

