/****************************************************************
              Copyright (C) 1986 by Manuel E. Bermudez
              Translated to C - 1991
*****************************************************************/

#include <stdio.h>
#include <header/Open_File.h>
#include <header/CommandLine.h>
#include <header/Table.h>
#include <header/Text.h>
#include <header/Error.h>
#include <header/String_Input.h> 
#include <header/Tree.h>
#include <header/Dcln.h>
#include <header/Constrainer.h>


#define ProgramNode    1
#define TypesNode      2
#define TypeNode       3
#define DclnsNode      4
#define VarNode        5
#define IntegerTNode   6
#define BooleanTNode   7
#define BlockNode      8
#define AssignNode     9
#define OutputNode     10
#define IfNode         11
#define WhileNode      12
#define NullNode       13
#define LENode         14
#define LTNode         15
#define GENode         16
#define GTNode         17
#define EqualNode      18
#define NotEqualNode   19
#define PlusNode       20
#define MinusNode      21
#define OrNode         22
#define MultNode       23
#define DivideNode     24
#define ModNode        25
#define AndNode        26
#define NotNode        27
#define ExpoNode       28
#define TrueNode       29
#define FalseNode      30
#define EofNode        31
#define ReadNode       32
#define IntegerNode    33
#define IdentifierNode 34
#define RepeatNode     35
#define LOOP_CTXT      36 
#define FOR_CTXT       37
#define LoopNode       38
#define ExitNode       39
#define SwapNode       40
#define ForUptoNode    41
#define ForDownNode    42
#define CaseNode       43
#define SeqNode        44
#define OtherwiseNode  45
#define CasesNode      46
#define CharacterTNode 47
#define CharacterNode  48
#define LITNode        49
#define ConstsNode     50
#define ConstNode      51
#define StringNode     52
#define SuccNode       53
#define PrecNode       54
#define OrdNode        55
#define ChrNode	       56
#define SubProgNode    57
#define FcnNode	       58
#define ParamsNode     59
#define CallNode       60
#define ReturnNode     61
#define SUB_CTXT       62
#define ProcedureNode  63


#define NumberOfNodes  63

typedef TreeNode UserType;

/****************************************************************
 Add new node names to the end of the array, keeping in strict
  order with the define statements above, then adjust the i loop
  control variable in InitializeConstrainer().
*****************************************************************/
char *node[] = { "program", "types", "type", "dclns",
                 "var", "integer", "boolean", "block",
                 "assign", "output", "if", "while", 
                 "<null>", "<=","<", ">=",">","=","<>", "+", "-", "or","*","/", "mod", "and","not", "**","true", "false", "eof","read",
                 "<integer>", "<identifier>","repeat", "<loop_ctxt>","<for_ctxt>", "loop", "exit", ":=:", "upto", "downto", "case", "..",  "otherwise", "case_clause","char","<char>", "lit", "consts","const","string","succ","pred","ord","chr","subprogs","function","params", "call","Return","<subprog_contxt>","procedure"};


UserType TypeInteger, TypeBoolean,TypeCharacter;
boolean TraceSpecified;
FILE *TraceFile;
char *TraceFileName;
int NumberTreesRead, index;


void Constrain(void)    
{
   int i;
   InitializeDeclarationTable();
   Tree_File = Open_File("_TREE", "r"); 
   NumberTreesRead = Read_Trees();
   fclose (Tree_File);                     

   AddIntrinsics();

#if 0
   printf("CURRENT TREE\n");
   for (i=1;i<=SizeOf(Tree);i++)
      printf("%2d: %d\n",i,Element(Tree,i));
#endif

   ProcessNode(RootOfTree(1)); 

    
   Tree_File = fopen("_TREE", "w");  
   Write_Trees();
   fclose (Tree_File);                   

   if (TraceSpecified)
      fclose(TraceFile);    
}


void InitializeConstrainer (int argc, char *argv[])
{          
   int i, j;

   InitializeTextModule();
   InitializeTreeModule();

   for (i=0, j=1; i<NumberOfNodes; i++, j++)
      String_Array_To_String_Constant (node[i], j); 
 
   index = System_Flag ("-trace", argc, argv);
 
   if (index)                                       
   {
      TraceSpecified = true; 
      TraceFileName = System_Argument ("-trace", "_TRACE", argc, argv);
      TraceFile = Open_File(TraceFileName, "w");
   }
   else
      TraceSpecified = false;          
}                        


void AddIntrinsics (void)
{
     TreeNode TempTree;

   AddTree (TypesNode, RootOfTree(1), 2);

   TempTree = Child (RootOfTree(1), 2);
   AddTree (TypeNode, TempTree, 1);

   TempTree = Child (RootOfTree(1), 2);
   AddTree (TypeNode, TempTree, 2);

   TempTree = Child (RootOfTree(1), 2);
   AddTree (TypeNode, TempTree, 3);

   TempTree = Child (Child (RootOfTree(1), 2), 1);
   AddTree (IdentifierNode, TempTree, 1);

   TempTree = Child (Child (Child (RootOfTree(1), 2), 1),1);
   AddTree (BooleanTNode, TempTree, 1);

   TempTree = Child (Child (RootOfTree(1), 2), 1);
   AddTree (LITNode, TempTree, 2);

   TempTree = Child (Child (Child (RootOfTree(1), 2), 1),2);
   AddTree (IdentifierNode, TempTree, 1);

   TempTree = Child (Child (Child (Child (RootOfTree(1), 2), 1),2),1);
   AddTree (FalseNode, TempTree, 1);

   TempTree = Child (Child (Child (RootOfTree(1), 2), 1),2);
   AddTree (IdentifierNode, TempTree, 2);

   TempTree = Child (Child (Child (Child (RootOfTree(1), 2), 1),2),2);
   AddTree (TrueNode, TempTree, 1);

   TempTree = Child (Child (RootOfTree(1), 2), 2);
   AddTree (IdentifierNode, TempTree, 1);

   TempTree = Child (Child (RootOfTree(1), 2), 3);
   AddTree (IdentifierNode, TempTree, 1);

   TempTree = Child (Child (Child (RootOfTree(1), 2), 2),1);
   AddTree (IntegerTNode, TempTree, 1);

   TempTree = Child (Child (Child (RootOfTree(1), 2), 3),1);
   AddTree (CharacterTNode, TempTree, 1);

}



void ErrorHeader (TreeNode T)
{ 
   printf ("<<< CONSTRAINER ERROR >>> AT ");
   Write_String (stdout,SourceLocation(T));
   printf (" : ");
   printf ("\n");
}


int NKids (TreeNode T)
{
   return (Rank(T));
}

int getMode(TreeNode T){
	
	TreeNode Declr= Lookup(NodeName(Child(T,1)),T);		
	int mode = NodeName(Decoration(Child(Declr,1)));	
	return mode;
}


UserType Expression (TreeNode T)
{
   UserType Type1, Type2;
   TreeNode Declaration, Temp1, Temp2;
   int NodeCount;

   if (TraceSpecified)
   {
      fprintf (TraceFile, "<<< CONSTRAINER >>> : Expn Processor Node ");
      Write_String (TraceFile, NodeName(T));
      fprintf (TraceFile, "\n");
   }
     
   switch (NodeName(T))
   {
      case LENode :    
         Type1 = Expression (Child(T,1));
         Type2 = Expression (Child(T,2));

         if (Type1 != Type2)
         {
            ErrorHeader(Child(T,1));
            printf ("ARGUMENTS OF '<=' MUST BE TYPE INTEGER\n");
            printf ("\n");
         }
         return (TypeBoolean);

      case LTNode :    
         Type1 = Expression (Child(T,1));
         Type2 = Expression (Child(T,2));

         if (Type1 != Type2)
         {
            ErrorHeader(Child(T,1));
            printf ("ARGUMENTS OF '<' MUST BE TYPE INTEGER\n");
            printf ("\n");
         }
         return (TypeBoolean);

      case GENode :    
         Type1 = Expression (Child(T,1));
         Type2 = Expression (Child(T,2));

         if (Type1 != Type2)
         {
            ErrorHeader(Child(T,1));
            printf ("ARGUMENTS OF '>=' MUST BE TYPE INTEGER \n");
            printf ("\n");
         }
         return (TypeBoolean);

      case GTNode :    
         Type1 = Expression (Child(T,1));
         Type2 = Expression (Child(T,2));

         if (Type1 != Type2)
         {
            ErrorHeader(Child(T,1));
            printf ("ARGUMENTS OF '>' MUST BE TYPE INTEGER\n");
            printf ("\n");
         }
         return (TypeBoolean);

      case EqualNode :  
  
         Type1 = Expression (Child(T,1));
         Type2 = Expression (Child(T,2));

         if (Type1 != Type2)
         {
            ErrorHeader(Child(T,1));
            printf ("ARGUMENTS OF '=' MUST BE TYPE INTEGER\n");
            printf ("\n");
         }
         return (TypeBoolean);

      case NotEqualNode :  
  
         Type1 = Expression (Child(T,1));
         Type2 = Expression (Child(T,2));

         if (Type1 != Type2)
         {
            ErrorHeader(Child(T,1));
            printf ("ARGUMENTS OF '!=' MUST BE TYPE INTEGER\n");
            printf ("\n");
         }
         return (TypeBoolean);

      case PlusNode :
      case MinusNode : 
         Type1 = Expression (Child(T,1));

         if (Rank(T) == 2)
            Type2 = Expression (Child(T,2));
         else  
            Type2 = TypeInteger;

         if (Type1 != TypeInteger || Type2 != TypeInteger  )
         {
            ErrorHeader(Child(T,1));
            printf ("ARGUMENTS OF '+', '-', '*', '/', mod ");
            printf ("Cannot add characters\n");
            printf ("\n");
         }
         return (TypeInteger);

      case OrNode :
  
         Type1 = Expression (Child(T,1));
         Type2 = Expression (Child(T,2));

         if (Type1 != TypeBoolean || Type2 != TypeBoolean )
         {
            ErrorHeader(Child(T,1));
            printf ("ARGUMENTS OF '+', '-', '*', '/', mod ");
            printf ("MUST BE TYPE INTEGER\n");
            printf ("\n");
         }
         return (TypeBoolean);

      case MultNode :
  
         Type1 = Expression (Child(T,1));
         Type2 = Expression (Child(T,2));

         if (Type1 != TypeInteger || Type2 != TypeInteger)
         {
            ErrorHeader(Child(T,1));
            printf ("ARGUMENTS OF '+', '-', '*', '/', mod ");
            printf ("MUST BE TYPE INTEGER\n");
            printf ("\n");
         }
         return (TypeInteger);

      case DivideNode :
  
         Type1 = Expression (Child(T,1));
         Type2 = Expression (Child(T,2));

         if (Type1 != TypeInteger || Type2 != TypeInteger)
         {
            ErrorHeader(Child(T,1));
            printf ("ARGUMENTS OF '+', '-', '*', '/', mod ");
            printf ("MUST BE TYPE INTEGER\n");
            printf ("\n");
         }
         return (TypeInteger);

      case ModNode :
  
         Type1 = Expression (Child(T,1));
         Type2 = Expression (Child(T,2));

         if (Type1 != TypeInteger || Type2 != TypeInteger)
         {
            ErrorHeader(Child(T,1));
            printf ("ARGUMENTS OF '+', '-', '*', '/', mod ");
            printf ("MUST BE TYPE INTEGER\n");
            printf ("\n");
         }
         return (TypeInteger);


      case AndNode :
  
         Type1 = Expression (Child(T,1));
         Type2 = Expression (Child(T,2));

         if (Type1 != TypeBoolean || Type2 != TypeBoolean)
         {
            ErrorHeader(Child(T,1));
            printf ("ARGUMENTS OF '+', '-', '*', '/', mod ");
            printf ("MUST BE TYPE INTEGER\n");
            printf ("\n");
         }
         return (TypeBoolean);


      case ExpoNode :
  
         Type1 = Expression (Child(T,1));
         Type2 = Expression (Child(T,2));

         if (Type1 != TypeInteger || Type2 != TypeInteger)
         {
            ErrorHeader(Child(T,1));
            printf ("ARGUMENTS OF '+', '-', '*', '/', mod ");
            printf ("MUST BE TYPE INTEGER\n");
            printf ("\n");
         }
         return (TypeInteger);

      case NotNode :
  
         Type1 = Expression (Child(T,1));
 	
	 if (Type1 != TypeBoolean)
         {
            ErrorHeader(Child(T,1));
            printf ("Not should be boolean type\n");
            printf ("\n");
         }
         return (TypeBoolean);
      

      case IntegerNode : 
	Decorate(T,TypeInteger);
         return (TypeInteger);

      case CharacterNode : 
	Decorate(T,TypeCharacter);	
         return (TypeCharacter);
	

      case StringNode : 
         return (TypeCharacter);

      case BooleanTNode : 
	  return (TypeBoolean);

      case TrueNode : 
         return (TypeBoolean);

      case FalseNode : 
         return (TypeBoolean);

      case EofNode : 
         return (TypeBoolean);

     case SeqNode:
         Type1 = Expression (Child(T,1));
         Type2 = Expression (Child(T,2));

         if (Type1 != Type2)
         {
            ErrorHeader(Child(T,1));
            printf ("ARGUMENTS OF 'SEQ' MUST BE TYPE INTEGER\n");
            printf ("\n");
         }
         return Type1;

	 case SuccNode :
		Type1 = Expression (Child(T,1));	
		return Type1;


	case PrecNode :
		Type1 = Expression (Child(T,1));	
		return Type1;


	case OrdNode :
		Type1 = Expression (Child(T,1));	
		return TypeInteger;

	case ChrNode :
		Type1 = Expression (Child(T,1));
		
		if (Type1 != TypeInteger)
         	{
         	 	ErrorHeader(Child(T,1));
          	  	printf ("argument of chr must be of type integer\n");
            		printf ("\n");
         	}
	
		return TypeCharacter;

      case IdentifierNode :
         Declaration = Lookup (NodeName(Child(T,1)), T);

         if (Declaration != NullDeclaration)
         {
		
        	Decorate (T, Declaration);
            return (Decoration(Declaration));
         }
         else 
            return (TypeInteger);

      case CallNode:

		Type1 = Expression (Child(T,1));
			
		int mode= getMode(Child(T,1));		
		
		Type2 = Decoration(Child(T,1));
		Type2 = Child(Decoration(Child(Type2,1)),2);
		int Kid=2;
		for(Kid=2;Kid<=NKids(T);Kid++){
			Expression(Child(T,Kid));	
		}


		/*Total kids  */
		Kid =1;
		int total_params =0;				
		for(Kid=1;Kid<=NKids(Type2);Kid++){
				
			total_params = NKids(Child(Type2,Kid))-1 + total_params;
			
		}
		int called_params = NKids(T)-1;

		if(called_params > total_params){
			ErrorHeader(Child(T,1));
          	  	printf ("Too many arguments \n");
            		printf ("\n");	
		}
		else if(called_params < total_params){
			ErrorHeader(Child(T,1));
          	  	printf ("Too few arguments \n");
            		printf ("\n");	
		}

		int i=2;
		for (Kid = 1; Kid <= NKids(Type2); Kid++){
			int k = 1;
			int n1 = Child(Type2,Kid);	
			for (k = 1; k < NKids(Child(Type2,Kid )); k++){   	
				
				int t = NodeName(Child(Child(n1,NKids(n1)),1));
				
				int Type3 = NodeName(Child(Child(Expression(Child(T,i)),1),1));	
				i++;
				if(Type3 !=t){
					ErrorHeader(Child(T,1));
          			  	printf ("Parameter Types are different Error\n");
            				printf ("\n");		
				}
					
			}
		}
		
	return 	Type1;	

	

      default :
         ErrorHeader(T);
         printf ( "UNKNOWN NODE NAME ");
         Write_String (stdout,NodeName(T));
         printf ("\n");

   }  /* end switch */
}  /* end Expression */




void ProcessNode (TreeNode T) 
{
   int Kid, N, mode;
   String Name1, Name2;
   TreeNode Type1, Type2, Type3, Temp;

   if (TraceSpecified)
   {
      fprintf (TraceFile,
               "<<< CONSTRAINER >>> : Stmt Processor Node ");
      Write_String (TraceFile, NodeName(T));
      fprintf (TraceFile, "\n");
   }

   switch (NodeName(T))
   {
      case ProgramNode : 
	 DTEnter(SUB_CTXT,T);
         DTEnter(FOR_CTXT, T);
	 DTEnter(LOOP_CTXT,T);	 
	OpenScope();
	 
         Name1 = NodeName(Child(Child(T,1),1));
         Name2 = NodeName(Child(Child(T,NKids(T)),1));

         if (Name1 != Name2)
         {
           ErrorHeader(T);
           printf ("PROGRAM NAMES DO NOT MATCH\n");
           printf ("\n");
         }

         for (Kid = 2; Kid <= NKids(T)-1; Kid++)
            	ProcessNode (Child(T,Kid));
         CloseScope();
         break;

      case SubProgNode:	
         for (Kid = 1; Kid <= NKids(T); Kid++)
		ProcessNode (Child(T,Kid));
	break;

      case FcnNode:
		
		if(NodeName(Child(Child(T,1),1))!=NodeName(Child(Child(T,8),1))){
			ErrorHeader(T);
        		printf ("Function name does not match\n");
           		printf ("\n");
		}
		
		if(NodeName(Child(Child(T,1),1))== NodeName(Child(Child(T,NKids(T)),1))){
			DTEnter(NodeName(Child(Child(T,1),1)),Child(T,1));
			OpenScope();
			DTEnter(SUB_CTXT,T);
			Type1 = Lookup (NodeName(Child(Child(T,3),1)),T);
			Decorate(Child(T,1),Decoration(Type1));
			Decorate( Child(Child(T,1),1),T);	
			ProcessNode (Child(T,2));
			ProcessNode (Child(T,4));
			ProcessNode (Child(T,5));
			ProcessNode (Child(T,6));
			ProcessNode (Child(T,7));
			CloseScope();
		}
	break;

      case ProcedureNode:
	
		if(NodeName(Child(Child(T,1),1))== NodeName(Child(Child(T,NKids(T)),1))){
			DTEnter(NodeName(Child(Child(T,1),1)),Child(T,1));
			OpenScope();
			DTEnter(SUB_CTXT,T);
			Decorate( Child(Child(T,1),1),T);	
			ProcessNode (Child(T,2));
			ProcessNode (Child(T,3));
			ProcessNode (Child(T,4));
			ProcessNode (Child(T,5));
			ProcessNode (Child(T,6));			
			CloseScope();



		}
	break;

	case CallNode:

		Type1 = Expression (Child(T,1));
			
		int mode= getMode(Child(T,1));		
		if(mode!=ProcedureNode){
			ErrorHeader(Child(T,1));
          	  	printf ("Not a procedure\n");
            		printf ("\n");	
		}
		Type2 = Decoration(Child(T,1));
		Type2 = Child(Decoration(Child(Type2,1)),2);
		int Kid=2;
		int previous= 0;
		int current = 0;
		for(Kid=2;Kid<=NKids(T);Kid++){

			Expression(Child(T,Kid));	

			current = Decoration(Child(T,Kid));	
			previous = Decoration(Child(T,Kid-1));			
			
			
			if(current == previous){
				ErrorHeader(Child(T,1));
          	  		printf ("Bad argument type\n");
            			printf ("\n");	
				
			}	
		}
		
	return 	Type1;	


      case ParamsNode:
		for (Kid = 1; Kid <= NKids(T); Kid++)
			ProcessNode (Child(T,Kid));		
	break;
 
	case ReturnNode:

		if(NKids(T)>0){	
			Type1 = Expression (Child(T,1));
			Type2 = Lookup(SUB_CTXT,T);
			
			if(NodeName(Type2)== ProcedureNode){
				ErrorHeader(Child(T,1));
		  	  	printf ("Procedure returns a value\n");
		    		printf ("\n");	
			}
				
			else if(NodeName(Type2)!= FcnNode){
				ErrorHeader(Child(T,1));
		  	  	printf ("Function is used as a variable\n");
		    		printf ("\n");	
			}
			else if(Decoration(Child(Type2,1))!=Type1){
				ErrorHeader(Child(T,1));
		  	  	printf ("Return type is different\n");
		    		printf ("\n");	
			}
		}
	break;
			

      case TypesNode :  
	
	
         for (Kid = 1; Kid <= NKids(T); Kid++)
            ProcessNode (Child(T,Kid));
	if(Rank(T)==3){
         	TypeBoolean = Child(T,1);
         	TypeInteger = Child(T,2);
	 	TypeCharacter = Child(T,3);
	}        
	 break;


      case TypeNode :

         DTEnter (NodeName(Child(Child(T,1),1)),Child(T,1),T);
	 Decorate (Child(T,1), T);
         Decorate (Child(Child(T,1),1),T);

	
      	 if (Rank(T) == 2){

	

		for(Kid =1; Kid<= NKids(Child(T,2));Kid++){
			DTEnter (NodeName(Child(Child(Child(T,2),Kid),1)),Child(Child(T,2),Kid),T);
			Decorate (Child(Child(T,2),Kid), T);

			Decorate(Child(Child(Child(T,2),Kid),1), Child(T,2));
		}

	}

         break;


      case DclnsNode :
         for (Kid = 1; Kid <= NKids(T); Kid++)
            ProcessNode (Child(T,Kid));
         break;


      case VarNode :

	 Name1 = NodeName (Child(Child(T, NKids(T)),1));
         Type1 = Lookup (Name1,T);
	

	 Decorate (Child(T,NKids(T)), Type1);

	mode = getMode(Child(T, NKids(T)));

	if(mode != TypeNode){
             ErrorHeader(T); 
             printf ("Declaration type must be of type\n");
             printf ("\n");		
	}


         for (Kid  = 1; Kid < NKids(T); Kid++)
         {
            DTEnter (NodeName(Child(Child(T,Kid),1)), Child(T,Kid), T);
	    Decorate (Child(Child(T,Kid),1), T);
	    Decorate (Child(T,Kid), Decoration(Type1));

         }
         break;

      case ConstsNode :
         for (Kid = 1; Kid <= NKids(T); Kid++){
	   ProcessNode (Child(T,Kid));
	}
         break;


      case ConstNode :

	mode = getMode(Child(T,2));

	if(NodeName(Child(Child(T,2),1)) == IntegerTNode || NodeName(Child(Child(T,2),1)) == CharacterTNode){
		
		ErrorHeader(T); 
                printf ("constants can't be assigned a type\n");
                printf ("\n");	
	}

	
	if(NodeName(Child(T,2))== IntegerNode){
		
		Type1  = Lookup(IntegerTNode,T);		
		Decorate (Child(T,1), Decoration(Type1));
	
	}
	else if(NodeName(Child(T,2))== CharacterNode){
		
		Type1  = Lookup(CharacterTNode,T);		
		Decorate (Child(T,1), Decoration(Type1));

	}	
	else{
		Name1 = NodeName (Child(Child(T, 2),1));
                Type1 = Lookup (Name1,T);
		Decorate(Child(T,1),Decoration(Type1));
	
  	        TreeNode Declaration = Lookup (NodeName(Child(Child(T, 2),1)), T);
  		Decorate(Child(T,2),Declaration);
	
	}
	DTEnter (NodeName(Child(Child(T,1),1)), Child(T,1), T);
	Decorate(Child(Child(T,1),1),T);
         break;	


      case BlockNode :
         for (Kid = 1; Kid <= NKids(T); Kid++)
            ProcessNode (Child(T,Kid));
         break;


      case AssignNode :


		 Type1 = Expression (Child(T,1));
		 Type2 = Expression (Child(T,2));
	

		int d1 = Decoration(Child(T,1));
		int d2 = Decoration(Child(d1,1));

		 if (NodeName(d2) == FcnNode && NKids(T)>1 )
		 {
		    ErrorHeader(T);
		    printf ("Function used as an variable\n");
		    printf ("\n");
		 }

		
		d1 = Decoration(Child(Child(T,2),1));
		d2 = Decoration(Child(d1,1));
			
	
		if (NodeName(d2) == ProcedureNode )
		 {
		    ErrorHeader(T);
		    printf ("y is not a function\n");
		    printf ("\n");
		 }


		 if (Type1 != Type2 )
		 {
		    ErrorHeader(T);
		    printf ("ASSIGNMENT TYPES DO NOT MATCH\n");
		    printf ("\n");
		 }


		/*mode = getMode(Child(T,2));*/


		if(mode == TypeNode){
		     ErrorHeader(T); 
		     printf ("Assignment cannot be of type \n");
		     printf ("\n");		
		}

	
		Type3 = Lookup(FOR_CTXT,T);
		while(NodeName(Type3) != ProgramNode){
				   
		    if (NodeName(Child(Child(Type3,1),1)) == NodeName(Child(Child(T,1),1))) {
			     ErrorHeader(T);
			        printf ("can't assign to loop control variable\n");
			        printf ("\n");
			 }
		    Type3 = Decoration(Type3);                          
		}                      
         break;


      case OutputNode :


         for (Kid = 1; Kid <= NKids(T); Kid++){
               if (Expression (Child(T,Kid)) == TypeBoolean)
               {
			ErrorHeader(T);
               		printf ("Cannot output boolean type\n");
               		printf ("\n");

               }		
		
		int d = Decoration(Child(T,Kid));
		
		if(NodeName(Decoration(Child(d,1)))== LITNode){

				ErrorHeader(T);
               		printf ("Wrong output type\n");
               		printf ("\n");


		}	

	}		
	         break;


	case ReadNode :

         for (Kid = 1; Kid <= NKids(T); Kid++)
            Expression (Child(T,Kid)) ;           
         break;
     


      case IfNode :
         if (Expression (Child(T,1)) != TypeBoolean)
         {
            ErrorHeader(T);
            printf ("CONTROL EXPRESSION OF 'IF' STMT");
            printf (" IS NOT TYPE BOOLEAN\n");
            printf ("\n");
         } 

         ProcessNode (Child(T,2));
         if (Rank(T) == 3)
            ProcessNode (Child(T,3));
         break;


      case WhileNode :
         if (Expression (Child(T,1)) != TypeBoolean)
         {
            ErrorHeader(T);
            printf ("WHILE EXPRESSION NOT OF TYPE BOOLEAN\n");
            printf ("\n");
         }
         ProcessNode (Child(T,2));
         break;

	case RepeatNode :
         if (Expression (Child(T,NKids(T))) != TypeBoolean)
         {
            ErrorHeader(T);
            printf ("Repeat expression not boolean\n");
            printf ("\n");
         }
	 for (Kid = 1; Kid < NKids(T); Kid++)
            ProcessNode (Child(T,Kid));
         break;


      case LoopNode : 
	OpenScope();
	DTEnter(LOOP_CTXT,T,T);  
	for (Kid = 1; Kid <= NKids(T); Kid++)
            ProcessNode (Child(T,Kid));
	CloseScope();
	if (Decoration(T)==0){
		 printf("Warning: no ‘exit’");
	}
	break;

      case ExitNode : 
	Type1 = Lookup(LOOP_CTXT,T);
	if(NodeName(Type1) !=LoopNode){
		ErrorHeader(T);
        	printf ("must exit from loop statement\n");
            	printf ("\n");
	}
	 Decorate(T,Type1);
	Decorate(Type1,T);
	break;

	case SwapNode : 
	
	Temp=Lookup(FOR_CTXT,T);
    	while(NodeName(Temp)!=ProgramNode){
           	if(NodeName(Child(Child(Temp,1),1))==NodeName(Child(Child(T,1),1))){
           		ErrorHeader(T);
           	 	printf ("SWAP CANT BE DONE ON LOOP CONTROL VARIABLE\n");
           	 	printf ("\n");
           	 	break;
	   	}
		else
		Temp=Decoration(Temp);
	}	

	 Type1 = Expression (Child(T,1));
         Type2 = Expression (Child(T,2));

         if (Type1 != TypeInteger || Type2 != TypeInteger)
         {
            ErrorHeader(T);
            printf ("ASSIGNMENT TYPES DO NOT MATCH\n");
            printf ("\n");
         }
         break;

	case ForUptoNode :

		Temp = Lookup(FOR_CTXT,T);
		Decorate (T,Temp);
		OpenScope();
		DTEnter(FOR_CTXT,T);
		DTEnter(LOOP_CTXT); 

		
		Type1 = Expression (Child(T,1));
		Type2 = Expression (Child(T,2));
		Type3 = Expression (Child(T,3));

	     if (Type1 != Type2)
	     {
	         ErrorHeader(T);
	            printf ("ASSIGNMENT TYPES DO NOT MATCH\n");
	            printf ("\n");
		
	     }

	     if (Type1 != Type3)
	     {
	         ErrorHeader(T);
	            printf ("ASSIGNMENT TYPES DO NOT MATCH\n");
	            printf ("\n");
		
	     }
	

		ProcessNode (Child(T,4));

		while(NodeName(Temp) != ProgramNode){
		           
		    if (NodeName(Child(Child(Temp,1),1)) == NodeName(Child(Child(T,1),1))) {
		             ErrorHeader(T);
		                printf ("Can't re-use loop control variable\n");
		                printf ("\n");
		         }
		    Temp = Decoration(Temp);                          
		}
	    CloseScope();
	    break;

	case ForDownNode: 
		
		Temp = Lookup(FOR_CTXT,T);
		Decorate (T,Temp);
		OpenScope();
		DTEnter(FOR_CTXT,T);

		Type1 = Expression (Child(T,1));
		Type2 = Expression (Child(T,2));
		Type3 = Expression (Child(T,3));
		

	     if (Type1 != Type2)
	     {
	         ErrorHeader(T);
	            printf ("ASSIGNMENT TYPES DO NOT MATCH\n");
	            printf ("\n");
		
	     }

	if (Type1 != Type3)
	     {
	         ErrorHeader(T);
	            printf ("ASSIGNMENT TYPES DO NOT MATCH\n");
	            printf ("\n");
		
	     }

		ProcessNode (Child(T,4));

		while(NodeName(Temp) != ProgramNode){
		           
		    if (NodeName(Child(Child(Temp,1),1)) == NodeName(Child(Child(T,1),1))) {
		             ErrorHeader(T);
		                printf ("Can't re-use loop control variable\n");
		                printf ("\n");
		         }
		    Temp = Decoration(Temp);                          
		}
	    CloseScope();
	    break;


	
	case CaseNode :

		Type1 = Expression(Child(T,1));


		 for(Kid=2;Kid<=NKids(T);Kid++){
			if(NodeName(Child(T,Kid))==CasesNode){
			Type2=Expression (Child(Child(T,Kid),1));
			if(Type2!=Type1)
			{
			ErrorHeader(Child(Child(T,Kid),1));
				    printf(" error: case variable and label mismatch");
				    printf ("\n");
			}
			ProcessNode(Child(Child(T,Kid),2));
			}
			else if(NodeName(Child(T,Kid))== OtherwiseNode){

			ProcessNode(Child(Child(T,Kid),1));
			}
		}
	break;

	
    

      case NullNode : 
         break;

      default :
         ErrorHeader(T);
         printf ("UNKNOWN NODE NAME ");
         Write_String (stdout,NodeName(T));
         printf ("\n");

   }  /* end switch */
}  /* end ProcessNode */
