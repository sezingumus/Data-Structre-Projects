       #include "binomial.h"
       #include "fatal.h"
       #include <string.h>
       #include <math.h>
       #include <stdlib.h>
/* Sezin Gumus
150113841
Data Structure Project 3*/
        typedef struct BinNode *Position;

        struct BinNode
        {
		    ElementType Item;//extime
            Position    LeftChild;
            Position    NextSibling;
        };
        struct Collection
        {
            int CurrentSize;
            BinTree TheTrees[ MaxTrees ];
        };

        BinQueue
        Initialize( void )
        {
            BinQueue H;
            int i;

            H = malloc( sizeof( struct Collection ) );
            if( H == NULL )
                FatalError( "Out of space!!!" );
            H->CurrentSize = 0;
            for( i = 0; i < MaxTrees; i++ )
                H->TheTrees[ i ] = NULL;
            return H;
        }

        static void
        DestroyTree( BinTree T )
        {
            if( T != NULL )
            {
                DestroyTree( T->LeftChild );
                DestroyTree( T->NextSibling );
                free( T );
            }
        }

        void
        Destroy( BinQueue H )
        {
            int i;

            for( i = 0; i < MaxTrees; i++ )
                DestroyTree( H->TheTrees[ i ] );
        }

        BinQueue
        MakeEmpty( BinQueue H )
        {
            int i;

            Destroy( H );
            for( i = 0; i < MaxTrees; i++ )
                H->TheTrees[ i ] = NULL;
            H->CurrentSize = 0;

            return H;
        }

        /* Not optimized for O(1) amortized performance */
        BinQueue
        Insert( ElementType Item,/*model_node_type m,*/ BinQueue H )
        {
            BinTree NewNode;
            BinQueue OneItem;
			int i;

            NewNode = malloc( sizeof( struct BinNode ) );
            if( NewNode == NULL )
                FatalError( "Out of space!!!" );
            NewNode->Item = Item;
            NewNode->LeftChild = NewNode->NextSibling = NULL;

            OneItem = Initialize( );
            OneItem->CurrentSize = 1;
            OneItem->TheTrees[ 0 ] = NewNode;

            return Merge( H, OneItem );
        }

        ElementType
        DeleteMin( BinQueue H )
        {
            int i, j;
            int MinTree;
            BinQueue DeletedQueue;
            Position DeletedTree, OldRoot;
            ElementType MinItem;

            if( IsEmpty( H ) )
            {
                Error( "Empty binomial queue" );
				MinItem=-Infinity;

                return MinItem;
            }

            MinItem = Infinity;
            for( i = 0; i < MaxTrees; i++ )
            {
                if( H->TheTrees[ i ] &&
                    H->TheTrees[ i ]->Item < MinItem )
                {

                    MinItem = H->TheTrees[ i ]->Item;

                    MinTree = i;
                }
            }

            DeletedTree = H->TheTrees[ MinTree ];
            OldRoot = DeletedTree;
            DeletedTree = DeletedTree->LeftChild;
            free( OldRoot );

            DeletedQueue = Initialize( );
            DeletedQueue->CurrentSize = ( 1 << MinTree ) - 1;
            for( j = MinTree - 1; j >= 0; j-- )
            {
                DeletedQueue->TheTrees[ j ] = DeletedTree;
                DeletedTree = DeletedTree->NextSibling;
                DeletedQueue->TheTrees[ j ]->NextSibling = NULL;
            }

            H->TheTrees[ MinTree ] = NULL;
            H->CurrentSize -= DeletedQueue->CurrentSize + 1;

            Merge( H, DeletedQueue );
            return MinItem;
        }

        ElementType
        FindMin( BinQueue H )
        {
            int i;
            ElementType MinItem;

            if( IsEmpty( H ) )
            {
                Error( "Empty binomial queue" );
				MinItem=0;
                return MinItem;
            }

            MinItem = Infinity;

            for( i = 0; i < MaxTrees; i++ )
            {
                if( H->TheTrees[ i ] &&
                            H->TheTrees[ i ]->Item < MinItem ) {
                    MinItem = H->TheTrees[ i ]->Item;
				}
            }

            return MinItem;
        }

        int
        IsEmpty( BinQueue H )
        {
            return H->CurrentSize == 0;
        }

        int IsFull( BinQueue H )
        {
            return H->CurrentSize == Capacity;
        }

        BinTree
        CombineTrees( BinTree T1, BinTree T2 )
        {
            if( T1->Item > T2->Item )
                return CombineTrees( T2, T1 );
            T2->NextSibling = T1->LeftChild;
            T1->LeftChild = T2;
            return T1;
        }

        BinQueue
        Merge( BinQueue H1, BinQueue H2 )
        {
            BinTree T1, T2, Carry = NULL;
            int i, j;

            if( H1->CurrentSize + H2->CurrentSize > Capacity )
                Error( "Merge would exceed capacity" );

            H1->CurrentSize += H2->CurrentSize;
            for( i = 0, j = 1; j <= H1->CurrentSize; i++, j *= 2 )
            {
                T1 = H1->TheTrees[ i ]; T2 = H2->TheTrees[ i ];

                switch( !!T1 + 2 * !!T2 + 4 * !!Carry )
                {
                    case 0: /* No trees */
                    case 1: /* Only H1 */
                        break;
                    case 2: /* Only H2 */
                        H1->TheTrees[ i ] = T2;
                        H2->TheTrees[ i ] = NULL;
                        break;
                    case 4: /* Only Carry */
                        H1->TheTrees[ i ] = Carry;
                        Carry = NULL;
                        break;
                    case 3: /* H1 and H2 */
                        Carry = CombineTrees( T1, T2 );
                        H1->TheTrees[ i ] = H2->TheTrees[ i ] = NULL;
                        break;
                    case 5: /* H1 and Carry */
                        Carry = CombineTrees( T1, Carry );
                        H1->TheTrees[ i ] = NULL;
                        break;
                    case 6: /* H2 and Carry */
                        Carry = CombineTrees( T2, Carry );
                        H2->TheTrees[ i ] = NULL;
                        break;
                    case 7: /* All three */
                        H1->TheTrees[ i ] = Carry;
                        Carry = CombineTrees( T1, T2 );
                        H2->TheTrees[ i ] = NULL;
                        break;
                }
            }
            return H1;
        }

		// by Borahan Tümer
		BinTree printTree(BinTree p, BinTree *r, int i)
		{
		  BinTree t[20]={NULL}, q; int j;
		  for ( j=0; j<i; j++ ) t[j]= r[j];
		  i=0;
		  if (p!=NULL) {
		    printf("& %2.1lf ",p->Item);
			q=p->NextSibling;
			j=0;
			do {
			  while (q!=NULL) {
			    printf("%2.1lf ",q->Item);
			    if (q->LeftChild != NULL) { r[i]=q->LeftChild; i++; }
			    q=q->NextSibling;
			  }
			  q=t[j++];
			} while (q!=NULL);
		  }
		  else return NULL;
		  //for (j=0; j<i; j++) t[j]=NULL;
		  printf("\n");
		  printTree(p->LeftChild, r, i);
		}
ElementType getMaxValue()  //To get max value from the file
{
          ElementType name [5];
		  ElementType et,ct,temp,max = 0;
		  int count = 0;
		  FILE *f;
          if((f = fopen("input.txt","r")) == NULL)
             printf("error");
          else
          {
              while(!feof(f))
              {
                fscanf(f,"%s%4lf%4lf",name,&et,&ct);
                if(et > max)
                { temp = et;
                  et = max;
                  max = temp;

                }
              }
              fclose(f);
          }
    return max;
}


void waitingtime() //It computes avarage time of process on the min binomial heap
{
    ElementType e_max = getMaxValue();
    ElementType q = 1.0,up;
    int count = 0;
    ElementType total = 0.0;
    ElementType time = 0.0,wt = 0.0;
    BinTree p, r[20]={NULL};
          BinQueue H1,H2,H3;
		   int i = 0;
          char n;
          char name [5];
		  ElementType et,ct;
           int id,*_ptr;
		  FILE *f;
		  H1 = Initialize( ); // for execution time of the process
		  H2 = Initialize( ); // for clock time of the process
		  H3 = Initialize( ); // for id of the process
          if((f = fopen("input.txt","r")) == NULL)
             printf("error");
          else
          {
              while(!feof(f))
              {

                    fscanf(f,"%s%4lf%4lf",name,&et,&ct);
                    sscanf(name,"%c%d",&n,&id);
                    count++;



                while(et > 0)
                {

                  if(et > q) // to check priority of the processes
                  {
                    q = (1.0/(exp(-pow(((2.0*(et-q))/(3.0*e_max)),3.0))))*(et-q);
                    wt++;
                    fscanf(f,"%s%4lf%4lf",name,&et,&ct);
                    sscanf(name,"%c%d",&n,&id);

                  Insert(id,H3);
                  Insert(et,H2);
                  Insert(ct,H3);
                   }

                  else
                  {
                      if(!IsEmpty(H1) && !IsEmpty(H2) &&!IsEmpty(H3)) //The list is empty or not
                      {

                          et = DeleteMin(H1);
                          ct = DeleteMin(H2);
                          id = DeleteMin(H3);
                      }
                        wt = 0;
                  }

                  et = et - q;
                  total += wt ;
                }


                         }

                     fclose(f);

     printf("Avarage waiting time :%lf",total/count); //Avarage waiting time
}
}


 		main()
		{
          waitingtime();
		}

