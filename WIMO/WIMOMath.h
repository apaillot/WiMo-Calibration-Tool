//============================================================================//
// Fichier .........: "WIMOMath.h"
// Auteur ..........: Christophe SCHAEFFER
// Date de création : 04/10/2018
//----------------------------------------------------------------------------//
/* Description :
    Gestion des calculs mathématiques.
*/
//============================================================================//

#ifndef WIMO_MATH_H
#define WIMO_MATH_H

//============================================================================//
// Inclusion des librairies
//============================================================================//
#include <TypeCMMN.h>
#include <TypeC.h>
//#include <TARGETHardware.h>

//#pragma pack(1)

//============================================================================//
// Déclaration des constantes
//============================================================================//

// Limites des paramètres physiques
#define aWIMO_MATH_PRESSURE_STATIC_MIN_VALUE      ( TFLOAT )-10.0
#define aWIMO_MATH_PRESSURE_STATIC_MAX_VALUE      ( TFLOAT )500.0
#define aWIMO_MATH_KELLER_PRESSURE_MIN_VALUE      ( TFLOAT )-10.0
#define aWIMO_MATH_KELLER_PRESSURE_MAX_VALUE      ( TFLOAT )500.0
#define aWIMO_MATH_KELLER_TEMPERATURE_MIN_VALUE   ( TFLOAT )-5.0
#define aWIMO_MATH_KELLER_TEMPERATURE_MAX_VALUE   ( TFLOAT )60.0
#define aWIMO_MATH_SALINITY_MIN_VALUE             ( TFLOAT )0.0
#define aWIMO_MATH_SALINITY_MAX_VALUE             ( TFLOAT )60.0
#define aWIMO_MATH_SOUND_VELOCITY_MIN_VALUE       ( TFLOAT )1300.0
#define aWIMO_MATH_SOUND_VELOCITY_MAX_VALUE       ( TFLOAT )1700.0
#define aWIMO_MATH_CONDUCTIVITY_25_MIN_VALUE      ( TFLOAT )0.0
#define aWIMO_MATH_CONDUCTIVITY_25_MAX_VALUE      ( TFLOAT )400.0
#define aWIMO_MATH_DISSOLVED_SOLIDS_MIN_VALUE     ( TFLOAT )0.0
#define aWIMO_MATH_DISSOLVED_SOLIDS_MAX_VALUE     ( TFLOAT )1000.0
#define aWIMO_MATH_CHLORIDE_MIN_VALUE             ( TFLOAT )0.0
#define aWIMO_MATH_CHLORIDE_MAX_VALUE             ( TFLOAT )1000.0
#define aWIMO_MATH_DEPTH_MIN_VALUE                ( TFLOAT )-10.0
#define aWIMO_MATH_DEPTH_MAX_VALUE                ( TFLOAT )500.0
#define aWIMO_MATH_OXYGEN_COMPENSATED_MIN_VALUE   ( TFLOAT )0.0
#define aWIMO_MATH_OXYGEN_COMPENSATED_MAX_VALUE   ( TFLOAT )15.0

//============================================================================//
// Déclaration des types
//============================================================================//

typedef struct
{
 TDOUBLE dCoeffA;
 TDOUBLE dCoeffB;
 TDOUBLE dCoeffC;
 TDOUBLE dCoeffD;
}
TWIMOMathCTNObject;

typedef struct
{
 TDOUBLE dCoeffA;
 TDOUBLE dCoeffB;
 TDOUBLE dCoeffC;
}
TWIMOMathPolynome2Object;

typedef struct
{
 TDOUBLE dCoeffA;
 TDOUBLE dCoeffB;
 TDOUBLE dCoeffC;
 TDOUBLE dCoeffD;
}
TWIMOMathPolynome3Object;

typedef struct
{
 TFLOAT fCoeffA;
 TFLOAT fCoeffB;
}
TWIMOMathLinearObject;

//#pragma pack(1)
/*
typedef struct
{
 TFLOAT fPhysical;
 TFLOAT fRaw;
}
TWIMOMathPointObject;
//#pragma pack(4)
*/
//============================================================================//
// Déclaration des variables exportées
//============================================================================//

//============================================================================//
// Déclaration des macros
//============================================================================//

//============================================================================//
// Déclaration des prototypes de fonctions exportées
//============================================================================//

//#pragma pack(4)

#endif
