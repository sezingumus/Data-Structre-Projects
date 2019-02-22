#include <stdio.h>
#include <stdlib.h>
#include <string.h>


//Score structure
struct Score{
	double score_number;
	int total_number_of_reviews;
	struct Review* reviewPtr;
	struct Score* right;
	struct Score* left;
};


//review structure
struct Review{
	int review_id;
   	char text[200];
	struct Review* right;
	struct Review* left;
};




typedef struct Score Score;
typedef struct Score* ScorePtr;
typedef struct Score** ScorePtrPtr;

typedef struct Review Review;
typedef struct Review* ReviewPtr;
typedef struct Review** ReviewPtrPtr;

//Prototype
Score * destroy(ScorePtr tree,double val);
Score * find_min(ScorePtr tree);
Score * search_score(ScorePtrPtr tree,double val);
Review * check_ID(ReviewPtrPtr tree,int id);

int main(){
//variables
ScorePtr head_score = NULL;
ScorePtr del;
ReviewPtr head_rev = NULL;
FILE *f;
int choice;
double score;
int id;
char word[300];
char text[200];

//to read Order.txt file
if((f=fopen("order.txt", "r"))==NULL){
		return 0;
	}
	while((fgets(word,300,f))!= NULL){
		sscanf(word,"%lf_%d",&score,&id);
        insert_score(&head_score,score);
        insert_ID(&head_rev,id);
		}
	if(f){
		fclose(f);
	}


    menu();
	printf("Enter your choice:");
	scanf("%d",&choice);

	while(choice != 7)
	{
	    switch(choice)
	    {
        case 1://Insert score , before the process , search score checks entered number
            //If search_scıre finds same score ,insert process is failed
            printf("Enter a new score :");
            scanf("%lf",&score);
            if(search_score(&head_score,score))
            {
                printf("The process is failed\n");
            }
            else
            {
                insert_score(&head_score,score);
                print_score(head_score);
            }
            break;
        case 2: //Add review process,before the process,
            //ID 's are checked to prevent adding same ID
            printf("Enter a score\n");
            scanf("%lf",&score);
            if(search_score(&head_score,score))
            {
             printf("Enter a new ID\n");
             scanf("%d",&id);
             if(!check_ID(&head_rev,id))
             {

             add_reviewscore(&head_score,&head_rev,score,id);
             }
             else
             {
                 printf("Existing ID , please enter a new ID\n");
                 while(head_rev->review_id == id)
                 {
                     printf("Enter a new ID");
                     scanf("%d",&id);
                 }
             }

            }
            else
                printf("Invalid score,the process is invalid");
            break;
        case 3:
            //findaverage()
            break;
        case 4:
            break;
        case 5:
            break;
        case 6:
            break;
        default:
            printf("Invalid choice\n");
            menu();
	        printf("\nEnter your choice:");
	        scanf("%d",&choice);
            break;
	    }
	    menu();
	    printf("Enter your choice:");
	    scanf("%d",&choice);

	}





}
void insert_score(ScorePtrPtr Tree, double val){
    ScorePtr temp = NULL;

    if(!(*Tree)){ //Tree is null
        temp = (Score*)malloc(sizeof(Score));
        temp->left = temp->right = NULL;
        temp->score_number = val;
        *Tree = temp;
        return;
    }

    if(val < ((*Tree)->score_number))
    {
        insert_score(&(*Tree)->left, val);
    }
    else if(val > ((*Tree)->score_number))
    {
        insert_score(&(*Tree)->right, val);

    }
}
//The method transfers Id s to linked list from Order txt
void insert_ID (ReviewPtrPtr tree,int id)
 {

     ReviewPtr temp = NULL;

     if(!(*tree))
     {
        temp = (Review*)malloc(sizeof(Review));
        temp->left = temp->right = NULL;
        temp->review_id = id;
        *tree = temp;
        return ;
     }
     if(id <((*tree)->review_id))
     {
         insert_ID(&(*tree)->left,id);
     }
     else if(id > ((*tree)->review_id))
     {
         insert_ID(&(*tree)->right,id);
     }
 }
 //This function checks and seeks score
Score * search_score(ScorePtrPtr tree,double val)
{
    if(!(*tree))
    {
        return ;
    }

    if(val < (*tree)->score_number)
    {
        search_score(&((*tree)->left), val);
    }
    else if(val > (*tree)->score_number)
    {
        search_score(&((*tree)->right), val);
    }
    else if(val == (*tree)->score_number)
    {
    return *tree;

    }

}
//This function checks and seeks ID s
Review * check_ID(ReviewPtrPtr tree,int val)
{
   if(!(*tree))
    {
        return ;
    }

    if(val < (*tree)->review_id)
    {
        check_ID(&((*tree)->left), val);
    }
    else if(val > (*tree)->review_id)
    {
        check_ID(&((*tree)->right), val);
    }
    else if(val == (*tree)->review_id)
    {

    return *tree;

    }
}

//This method adds a new review
void add_reviewscore(ScorePtrPtr tree,ReviewPtrPtr tr,double score,int id,char text[200])
{


    if((*tree))
    {
        (*tree)->reviewPtr = tr;

        if(search_score(&(*tree),(*tree)->score_number))
    {

      if((*tr)->review_id > id)
      {
          insert_ID(&(*tr)->left,id);

      }
      else
      {
          insert_ID(&(*tr)->right,id);
      }
    }
    else
    {
        printf("The process is failed");
    }
    }

}
//This method displays the scores
void print_score(ScorePtr tree){
    if (tree)
    {
        print_score(tree->right);
        printf("\nScore:%lf\n",tree->score_number);
        //print_ID(tree->reviewPtr);
        print_score(tree->left);
    }

}
//This method displays ID's
void print_ID(ReviewPtr tree)
{
    if (tree)
    {
          print_ID(tree->left);

        printf("Review:%d\n",tree->review_id);

        print_ID(tree->right);
    }

}
Score *destroy(ScorePtr tree,double val)
{
    ScorePtr temp;
    if(!tree)
       printf("The list is empty");
    if (val < tree->score_number)
        tree->left = destroy(tree->left,val);
    else if (val > tree->score_number)
        tree->right = destroy(tree->right,val);
    else
    {
        if (tree->right && tree->left)
        {
            temp = find_min(tree->right);
            tree->score_number = temp->score_number;
            tree->right = destroy(tree->right,tree->score_number);
        }
        else
        {
            temp = tree;
            if(tree->left == NULL)
                tree = tree->right;
            else if(tree->right == NULL)
                tree = tree->left;

            free(temp);
        }
        return tree;
    }

}
Score * find_min(ScorePtr tree)
{
    if(!tree)
        return NULL;
    if(tree->left == NULL)
        return tree;
    return find_min(tree->left);
}

//Displays the menu
void menu()
{
    printf("1:Add a new review-score\n"
           "2:Add some reviews into a review-score\n"
           "3:Calculate the average score of all of the reviews\n"
           "4:List the id s of reviews which have a particular word\n"
           "5:Remove the reviews between in a specific score-scope\n"
           "6:Display\n"
           "7:EXIT\n");
}





