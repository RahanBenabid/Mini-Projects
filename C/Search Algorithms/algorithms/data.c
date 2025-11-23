/* Include the header that contains the extern declaration
 * in every .c file where the variable is referenced, even
 * the file containing the variable definition.
 */

// those are READ-ONLY
#include "data.h"

char *pattern = "tower";
//char *pattern = "ataatata";

char *text = "In the quiet valley stood a tall tower, taller than any other "
             "tower in the land.  Every morning, the tower’s shadow stretched "
             "across the sleepy village,  and the villagers whispered tales of "
             "the tower’s origin. Some said it was built  by kings, others by "
             "wizards. But no one truly knew the truth behind the tower. ";
