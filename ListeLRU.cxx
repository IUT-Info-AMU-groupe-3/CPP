/**
 *
 * @file     ListeLRU.cxx
 *
 * @authors  M. Laporte, D. Mathieu
 *
 * @date     02/04/2010
 *
 * @version  V1.0
 *
 * @brief    Liste LRU
 *
 **/
#include <iostream>
#include <sstream>
#include <cassert>
#include <exception>

#include "C2Links.h"
#include "CstCodErr.h"
#include "CException.h"

using namespace std;
using namespace nsUtil;
using namespace nsSdD;

namespace 
{
    Ptr2L_t Find (Ptr2L_t Tete, Ptr2L_t Queue, int Val)
    {
        Ptr2L_t Ptr = Tete;
        Ptr=Ptr->GetSuivant();
        // ToDo
        for ( ; (Ptr = Ptr->GetSuivant())->GetSuivant(); )
                {
                    if (Ptr->GetInfo() == Val)
                    {
                        Ptr->GetPrecedent()->SetSuivant(Ptr->GetSuivant()); //Le suivant de mon précédent est mon suivant
                        Ptr->GetSuivant()->SetPrecedent(Ptr->GetPrecedent()); //Le précédent de mon suivant est mon précédent
                        Ptr->SetPrecedent(Tete->GetPrecedent()); //étape 3
                        Ptr->SetSuivant(Tete->GetSuivant()); //étape 4
                        Tete->GetPrecedent()->SetPrecedent(Ptr->GetSuivant());//On déconnecte la tête de la sentinelle et on la connecte à la valeur trouvé
                        Tete->GetSuivant()->SetSuivant(Ptr->GetPrecedent());//Pareil que ligne précédente
                        return Ptr;
                    }
                }
        return Ptr;

    } // Find()

    Ptr2L_t Inserer (Ptr2L_t Tete, Ptr2L_t Queue,
                     const unsigned TailleMax, unsigned & Taille, 
                     int Val)
    {
        // ToDo
        


    } // Inserer()

    string ALEndroit (Ptr2L_t Tete, Ptr2L_t Queue)
    {
        ostringstream os;
        for (Ptr2L_t Ptr = Tete->GetSuivant(); 
             Ptr != Queue; Ptr = Ptr->GetSuivant ())
            os << Ptr->GetInfo() << "; ";

        return os.str();

    } // ALEndroit()

    string ALEnvers (Ptr2L_t Tete, Ptr2L_t Queue)
    {
        ostringstream os;
        for (Ptr2L_t Ptr = Queue->GetPrecedent(); 
            Ptr != Tete; Ptr = Ptr->GetPrecedent ())
            os << Ptr->GetInfo() << "; ";

        return os.str();

    } // ALEnvers()

    void ListeLRU (void)
    {
        cout << "ListeLRU\n";

        Ptr2L_t Tete  = new C2Links ();
        Ptr2L_t Queue = new C2Links (0, Tete);
        Tete->SetSuivant (Queue);

        const unsigned KCapacite = 10;
        unsigned Size = 0;

        for (unsigned i (10); i--; ) 
            Inserer (Tete, Queue, KCapacite, Size, i);

        // Liste initiale

        assert (ALEndroit (Tete, Queue) == 
                       "0; 1; 2; 3; 4; 5; 6; 7; 8; 9; ");
        assert (ALEnvers  (Tete, Queue) == 
                       "9; 8; 7; 6; 5; 4; 3; 2; 1; 0; ");

        Find (Tete, Queue, 0);
        assert (ALEndroit (Tete, Queue) == 
                       "0; 1; 2; 3; 4; 5; 6; 7; 8; 9; ");
        assert (ALEnvers  (Tete, Queue) == 
                       "9; 8; 7; 6; 5; 4; 3; 2; 1; 0; ");

        Find (Tete, Queue, 1);
        assert (ALEndroit (Tete, Queue) == 
                       "1; 0; 2; 3; 4; 5; 6; 7; 8; 9; ");
        assert (ALEnvers  (Tete, Queue) == 
                       "9; 8; 7; 6; 5; 4; 3; 2; 0; 1; ");

        Find (Tete, Queue, 5);
        assert (ALEndroit (Tete, Queue) == 
                       "5; 1; 0; 2; 3; 4; 6; 7; 8; 9; ");
        assert (ALEnvers  (Tete, Queue) == 
                       "9; 8; 7; 6; 4; 3; 2; 0; 1; 5; ");

        Find (Tete, Queue, 9);
        assert (ALEndroit (Tete, Queue) == 
                       "9; 5; 1; 0; 2; 3; 4; 6; 7; 8; ");
        assert (ALEnvers  (Tete, Queue) == 
                       "8; 7; 6; 4; 3; 2; 0; 1; 5; 9; ");

        Find (Tete, Queue, 8);
        assert (ALEndroit (Tete, Queue) == 
                       "8; 9; 5; 1; 0; 2; 3; 4; 6; 7; ");
        assert (ALEnvers  (Tete, Queue) == 
                       "7; 6; 4; 3; 2; 0; 1; 5; 9; 8; ");

        Find (Tete, Queue, 9);
        assert (ALEndroit (Tete, Queue) == 
                       "9; 8; 5; 1; 0; 2; 3; 4; 6; 7; ");
        assert (ALEnvers  (Tete, Queue) == 
                       "7; 6; 4; 3; 2; 0; 1; 5; 8; 9; ");

        // Ajout d'elements au-dela de la capacite maximale

        for (unsigned i = 21; i < 25; ++i)
            assert (i == Inserer (Tete, Queue,
                                  KCapacite, Size, i)->GetInfo()); 

        assert (ALEndroit (Tete, Queue) == 
                        "24; 23; 22; 21; 9; 8; 5; 1; 0; 2; ");

        cout << "OK\n";

        // Suppression des elements restants

        for (Ptr2L_t Ptr (Tete), Suiv; Ptr; Ptr = Suiv)
        {
            Suiv = Ptr->GetSuivant();
            delete Ptr;
        }

    }// ListeLRU ()

} // namespace

int main (int argc, char * argv[])
{
    try
    {
        ListeLRU();
    }
    catch (const CException & E)
    {
        cerr << "Erreur        : " << E.GetLibelle() << '\n'
             << "Code d'erreur = " << E.GetCodErr() << '\n';
        return E.GetCodErr();
    }
    catch (const exception & E)
    {
        cerr << "Exception standard : " << E.what() << '\n';
        return KExcStd;
    }
    catch (...)
    {
        cerr << "Exception inconnue\n";
        return KExcInconnue;
    }
    return KNoExc;

} // main()
