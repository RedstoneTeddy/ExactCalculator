#include "constants.hpp"
#include <string>
#include <vector>
#include "../calculation/number.hpp"
#include "../calculation/base_structures.hpp"


Number Constant::GetValue() {
    Number output(maxSignificant);
    output.SetNegative(false);

    // Source for Pi: http://www.geom.uiuc.edu/~huberty/math5337/groupe/digits.html
    if (name == "Pi") {
        output.SetFromString("3.1415926535897932384626433832795028841971693993751058209749445923078164062862089986280348253421170679");
    
        // Source for e: https://oeis.org/A001113
    } else if (name == "E") {
        output.SetFromString("2.718281828459045235360287471352662497757247093699959574966967627724076630353547594571382178525166427");
    
        // Source for g: https://de.wikipedia.org/wiki/Gravitationskonstante
    } else if (name == "G") {
        output.SetFromString("6.6743015e-11");
    
    // Source for c: https://www.leifiphysik.de/optik/lichtausbreitung/grundwissen/lichtgeschwindigkeit
    } else if (name == "C") {
        output.SetFromString("299792458");
        
    // Source for u: https://www.studienkreis.de/chemie/atomare-masseneinheit-u/
    } else if (name == "U") {
        output.SetFromString("1.66053906660e-27");
    }

    output.CorrectForSignificance();
    return output;
}


