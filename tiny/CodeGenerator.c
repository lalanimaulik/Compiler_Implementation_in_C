/*******************************************************************
          Copyright (C) 1986 by Manuel E. Bermudez
          Translated to C - 1991
********************************************************************/

#include <stdio.h>
#include <header/CommandLine.h>
#include <header/Open_File.h>
#include <header/Table.h>
#include <header/Text.h>
#include <header/Error.h>
#include <header/String_Input.h>
#include <header/Tree.h>
#include <header/Code.h>
#include <header/CodeGenerator.h>  
#define LeftMode 0
#define RightMode 1

    /*  ABSTRACT MACHINE OPERATIONS  */
#define    NOP          1   /* 'NOP'       */
#define    HALTOP       2   /* 'HALT'      */      
#define    LITOP        3   /* 'LIT'       */
#define    LLVOP        4   /* 'LLV'       */
#define    LGVOP        5   /* 'LGV'       */
#define    SLVOP        6   /* 'SLV'       */
#define    SGVOP        7   /* 'SGV'       */
#define    LLAOP        8   /* 'LLA'       */
#define    LGAOP        9   /* 'LGA'       */
#define    UOPOP       10   /* 'UOP'       */
#define    BOPOP       11   /* 'BOP'       */     
#define    POPOP       12   /* 'POP'       */  
#define    DUPOP       13   /* 'DUP'       */
#define    SWAPOP      14   /* 'SWAP'      */
#define    CALLOP      15   /* 'CALL'      */
#define    RTNOP       16   /* 'RTN'       */
#define    GOTOOP      17   /* 'GOTO'      */
#define    CONDOP      18   /* 'COND'      */
#define    CODEOP      19   /* 'CODE'      */
#define    SOSOP       20   /* 'SOS'       */
#define    LIMITOP     21   /* 'LIMIT'     */

    /* ABSTRACT MACHINE OPERANDS */

         /* UNARY OPERANDS */
#define    UNOT        22   /* 'UNOT'     */
#define    UNEG        23   /* 'UNEG'     */
#define    USUCC       24   /* 'USUCC'    */
#define    UPRED       25   /* 'UPRED'    */
         /* BINARY OPERANDS */
#define    BAND        26   /* 'BAND'     */
#define    BOR         27   /* 'BOR'      */
#define    BPLUS       28   /* 'BPLUS'    */
#define    BMINUS      29   /* 'BMINUS'   */
#define    BMULT       30   /* 'BMULT'    */
#define    BDIV        31   /* 'BDIV'     */
#define    BEXP        32   /* 'BEXP'     */
#define    BMOD        33   /* 'BMOD'     */
#define    BEQ         34   /* 'BEQ'      */
#define    BNE         35   /* 'BNE'      */
#define    BLE         36   /* 'BLE'      */
#define    BGE         37   /* 'BGE'      */
#define    BLT         38   /* 'BLT'      */
#define    BGT         39   /* 'BGT'      */
         /* OS SERVICE CALL OPERANDS */
#define    TRACEX      40   /* 'TRACEX'   */
#define    DUMPMEM     41   /* 'DUMPMEM'  */
#define    OSINPUT     42   /* 'INPUT'    */
#define    OSINPUTC    43   /* 'INPUT'    */
#define    OSOUTPUT    44   /* 'OUTPUT'   */
#define    OSOUTPUTC   45   /* 'OUTPUT'   */
#define    OSOUTPUTL   46   /* 'OUTPUTL'  */
#define    OSEOF       47   /* 'EOF'      */

         /* TREE NODE NAMES */
#define    ProgramNode  48   /* 'program'  */
#define    TypesNode    49   /* 'types'    */
#define    TypeNode     50   /* 'type'     */
#define    DclnsNode    51   /* 'dclns'    */
#define    VarNode      52   /* 'dcln'     */
#define    IntegerTNode 53   /* 'integer'  */
#define    BooleanTNode 54   /* 'boolean'  */
#define    BlockNode    55   /* 'block'    */
#define    AssignNode   56   /* 'assign'   */
#define    OutputNode   57   /* 'output'   */ 
#define    IfNode       58   /* 'if'       */
#define    WhileNode    59   /* 'while'    */
#define    NullNode     60   /* '<null>'   */
#define    LENode       61   /* '<='       */
#define    LTNode       62   /* '<'        */
#define    GENode       63   /* '>='       */
#define    GTNode       64   /* '>'        */
#define    EqualNode 	65   /* '='        */
#define    NotEqualNode 66   /* '<>'       */
#define    PlusNode     67   /* '+'        */
#define    MinusNode    68   /* '-'        */
#define    OrNode 	69   /* 'or'       */
#define    MultNode     70   /* '*'        */
#define    DivideNode 	71   /* '/'        */
#define    ModNode 	72   /* 'mod'      */
#define    AndNode  	73   /* 'and'      */
#define    NotNode  	74   /* 'not'      */
#define    ExpoNode	75   /* '**'       */
#define    TrueNode     76   /* '<boolean>'*/
#define    FalseNode    77   /* '<boolean>'*/
#define    EofNode      78   /* '<eof>'    */
#define    ReadNode     79   /* 'read'     */
#define    IntegerNode  80   /* '<integer>'*/
#define    IdentifierNode 81 /* '<identifier>'*/
#define    RepeatNode   82   /* 'repeat'*/
#define    LoopNode   	83   /* 'loop'*/
#define    ExitNode   	84   /* 'exit'*/
#define    SwapNode   	85   /* 'swap'*/
#define    UptoForNode  86   /* 'uptofor'*/
#define    DownForNode  87   /* 'downfor'*/
#define    CaseNode     88   /* 'case'*/
#define    SequenceNode 89
#define    OtherwiseNode 90
#define    CasesNode    91
#define    CharacterNode 92    /* 'char'*/
#define    CharacterTNode 93    /* 'char'*/
#define    LITNode 	94
#define    ConstsNode 	95
#define    ConstNode 	96
#define    StringNode 	97
#define    SuccNode 	98
#define    PrecNode 	99
#define    OrdNode 	100
#define    ChrNode 	101
#define    SubProgNode  102
#define    FcnNode	103
#define    ParamsNode   104
#define    CallNode     105
#define    ReturnNode   106
#define    ProcedureNode   107


#define    NumberOfNodes 107 /* '<identifier>'*/
typedef int Mode;

FILE *CodeFile;
char *CodeFileName;
Clabel HaltLabel;

char *mach_op[] = 
    {"NOP","HALT","LIT","LLV","LGV","SLV","SGV","LLA","LGA",
     "UOP","BOP","POP","DUP","SWAP","CALL","RTN","GOTO","COND",
     "CODE","SOS","LIMIT","UNOT","UNEG","USUCC","UPRED","BAND",
     "BOR","BPLUS","BMINUS","BMULT","BDIV","BEXP","BMOD","BEQ",
     "BNE","BLE","BGE","BLT","BGT","TRACEX","DUMPMEM","INPUT",
     "INPUTC","OUTPUT","OUTPUTC","OUTPUTL","EOF"};

/****************************************************************** 
   add new node names to the end of the array, keeping in strict order
   as defined above, then adjust the j loop control variable in
   InitializeNodeNames(). 
*******************************************************************/
char *node_name[] =
    {"program","types","type","dclns","var","integer",
     "boolean","block","assign","output","if","while",
     "<null>","<=", "<",">=",">","=","<>", "+", "-" , "or","*","/", "mod", "and","not", "**", "true", "false","eof","read","<integer>", "<identifier>", "repeat","loop", "exit",":=:", "upto", "downto", "case", "..", "otherwise", "case_clause","<char>", "char","lit", "consts", "const", "string", "succ", "pred", "ord","chr","subprogs","function","params", "call","Return","procedure"};


void CodeGenerate(int argc, char *argv[])
{
   int NumberTrees;

   InitializeCodeGenerator(argc,argv);
   Tree_File = Open_File("_TREE", "r"); 
   NumberTrees = Read_Trees();
   fclose (Tree_File);                     

   HaltLabel = ProcessNode (RootOfTree(1), NoLabel);
   CodeGen0 (HALTOP, HaltLabel); 

   CodeFile = Open_File (CodeFileName, "w");
   DumpCode (CodeFile);
   fclose(CodeFile); 

   if (TraceSpecified)
      fclose (TraceFile);

/****************************************************************** 
  enable this code to write out the tree after the code generator
  has run.  It will show the new decorations made with MakeAddress().
*******************************************************************/


   Tree_File = fopen("_TREE", "w");  
   Write_Trees();
   fclose (Tree_File);                   
}


void InitializeCodeGenerator(int argc,char *argv[])
{
   InitializeMachineOperations();
   InitializeNodeNames();
   FrameSize = 0;
   CurrentProcLevel = 0;
   LabelCount = 0;
   CodeFileName = System_Argument("-code", "_CODE", argc, argv); 
}


void InitializeMachineOperations(void)
{
   int i,j;

   for (i=0, j=1; i < 47; i++, j++)
      String_Array_To_String_Constant (mach_op[i],j);
}



void InitializeNodeNames(void)
{
   int i,j;

   for (i=0, j=48; j <= NumberOfNodes; i++, j++)
      String_Array_To_String_Constant (node_name[i],j);
}



String MakeStringOf(int Number)
{
   Stack Temp;

   Temp = AllocateStack (50);
   ResetBufferInTextTable();
   if (Number == 0)
      AdvanceOnCharacter ('0');
   else
   {
      while (Number > 0)
      {
         Push (Temp,(Number % 10) + 48);
         Number /= 10;
      }

      while ( !(IsEmpty (Temp)))
         AdvanceOnCharacter ((char)(Pop(Temp)));
   }   
   return (ConvertStringInBuffer()); 
}  



void Reference(TreeNode T, Mode M, Clabel L)
{
   int Addr,OFFSET;
   String  Op;

   Addr = Decoration(Decoration(T));
   OFFSET = FrameDisplacement (Addr) ;
   switch (M)
   {
      case LeftMode  :  DecrementFrameSize();
                        if (ProcLevel (Addr) == 0) 
                           Op = SGVOP;
                        else
                           Op = SLVOP;
	                break;
      case RightMode :  IncrementFrameSize();
                        if (ProcLevel (Addr) == 0) 
                           Op = LGVOP;
          	        else
                           Op = LLVOP;
                        break;
   }
   CodeGen1 (Op,MakeStringOf(OFFSET),L);
}



int NKids (TreeNode T)
{
   return (Rank(T));
}

int getMode(TreeNode T){


	int mode = NodeName(Decoration(Child(Decoration(T),1)));
	return mode;

}


void Expression (TreeNode T, Clabel CurrLabel)
{
   int Kid,mode,value;
   Clabel Label1;

   if (TraceSpecified)
   {
      fprintf (TraceFile, "<<< CODE GENERATOR >>> Processing Node ");
      Write_String (TraceFile, NodeName (T) );
      fprintf (TraceFile, " , Label is  ");
      Write_String (TraceFile, CurrLabel);
      fprintf (TraceFile, "\n");
   }

   switch (NodeName(T))
   {
      case LENode :
         Expression ( Child(T,1) , CurrLabel);
         Expression ( Child(T,2) , NoLabel);
         CodeGen1 (BOPOP, BLE, NoLabel);
         DecrementFrameSize();
         break;

      case PlusNode :
         Expression ( Child(T,1) , CurrLabel);
         if (Rank(T) == 2){
            Expression ( Child(T,2) , NoLabel);
            CodeGen1 (BOPOP, BPLUS, NoLabel);
            DecrementFrameSize();
	}
         else
            CodeGen0 (NOP, NoLabel);
         break;

      case MultNode :
         Expression ( Child(T,1) , CurrLabel);
         Expression ( Child(T,2) , NoLabel);
         CodeGen1 (BOPOP, BMULT, NoLabel);
            
         DecrementFrameSize();
         break;

      case DivideNode :
         Expression ( Child(T,1) , CurrLabel);
         Expression ( Child(T,2) , NoLabel);
         CodeGen1 (BOPOP, BDIV, NoLabel);
            
         DecrementFrameSize();
         break;

     case ModNode :
         Expression ( Child(T,1) , CurrLabel);
         Expression ( Child(T,2) , NoLabel);
         CodeGen1 (BOPOP, BMOD, NoLabel);
            
         DecrementFrameSize();
         break;

      case AndNode :
         Expression ( Child(T,1) , CurrLabel);
         Expression ( Child(T,2) , NoLabel);
         CodeGen1 (BOPOP, BAND, NoLabel);
            
         DecrementFrameSize();
         break;

      case GENode :
         Expression ( Child(T,1) , CurrLabel);
         Expression ( Child(T,2) , NoLabel);
         CodeGen1 (BOPOP, BGE, NoLabel);
         DecrementFrameSize();
         break;

      case GTNode :
         Expression ( Child(T,1) , CurrLabel);
         Expression ( Child(T,2) , NoLabel);
         CodeGen1 (BOPOP, BGT, NoLabel);
         DecrementFrameSize();
         break;

      case EqualNode :
         Expression ( Child(T,1) , CurrLabel);
         Expression ( Child(T,2) , NoLabel);
         CodeGen1 (BOPOP, BEQ, NoLabel);
         DecrementFrameSize();
         break;

      case NotEqualNode :
         Expression ( Child(T,1) , CurrLabel);
         Expression ( Child(T,2) , NoLabel);
         CodeGen1 (BOPOP, BNE, NoLabel);
         DecrementFrameSize();
         break;

      case LTNode :
         Expression ( Child(T,1) , CurrLabel);
         Expression ( Child(T,2) , NoLabel);
         CodeGen1 (BOPOP, BLT, NoLabel);
         DecrementFrameSize();
         break;

      case MinusNode :
         Expression ( Child(T,1) , CurrLabel);
         if (Rank(T) == 2)
         {
            Expression ( Child(T,2) , NoLabel);
            CodeGen1 (BOPOP, BMINUS, NoLabel);
            DecrementFrameSize();
         }
         else
            CodeGen1 (UOPOP, UNEG, NoLabel);
         break;

      case NotNode :
         Expression ( Child(T,1) , CurrLabel);
         CodeGen1 (UOPOP, UNOT, NoLabel);
         break;
 
      case OrNode :
         Expression ( Child(T,1) , CurrLabel);
         Expression ( Child(T,2) , NoLabel);
         CodeGen1 (BOPOP, BOR, NoLabel);
            
         DecrementFrameSize();
         break;

      case ExpoNode :
         Expression ( Child(T,1) , CurrLabel);
         Expression ( Child(T,2) , NoLabel);
         CodeGen1 (BOPOP, BEXP, NoLabel);
            
         DecrementFrameSize();
         break;


      case IntegerNode :
         CodeGen1 (LITOP, NodeName (Child(T,1)), CurrLabel);
         IncrementFrameSize();
         break;

      case CharacterNode :
         CodeGen1 (LITOP, MakeStringOf(Character(NodeName (Child(T,1)),2)), CurrLabel);
         IncrementFrameSize();
         break;


      case TrueNode :
         CodeGen1 (LITOP, MakeStringOf(1), CurrLabel); 
         IncrementFrameSize();
         break;

      case FalseNode :
         CodeGen1 (LITOP, MakeStringOf(0), CurrLabel);
         IncrementFrameSize();
         break;


      case EofNode :
         CodeGen1 (SOSOP, OSEOF, CurrLabel);
         IncrementFrameSize();
         break;


       case SuccNode :

	Expression (Child(T,1), CurrLabel);
	CodeGen1(UOPOP,USUCC, NoLabel);
	break;

	case PrecNode :

	Expression (Child(T,1), CurrLabel);
	CodeGen1(UOPOP,UPRED, NoLabel);
	break;


	case OrdNode :

	Expression (Child(T,1), CurrLabel);

	break;

	case ChrNode :

	Expression (Child(T,1), CurrLabel);

	break;


      case IdentifierNode :
	
	mode = getMode(T);

	value = Decoration(Decoration(T));
	
	if(mode == LITNode){	
		CodeGen1 (LITOP, value, CurrLabel);
		IncrementFrameSize();
	}
	else if(mode == ConstNode){	
		CodeGen1 (LITOP, value, CurrLabel);
		IncrementFrameSize();
	}
	else{
		
         Reference (T,RightMode,CurrLabel);

	}         

	break;

	 case CallNode:
		
		CodeGen1 (LITOP,MakeStringOf(0), CurrLabel);
		IncrementFrameSize();		
		Clabel Label2 = Decoration(Decoration(Child(Decoration(Child(T,1)),1)));
		for (Kid = 2; Kid <= NKids(T); Kid++){
			Expression (Child(T,Kid), NoLabel);	
		}
		CodeGen1 (CODEOP,Label2, NoLabel);		
		
		for (Kid = 2; Kid <= NKids(T); Kid++){

			DecrementFrameSize();
		}
		
		CodeGen1 (CALLOP,MakeStringOf(FrameSize-1), NoLabel);		
			
	break;


      default :
         ReportTreeErrorAt(T);
         printf ("<<< CODE GENERATOR >>> : UNKNOWN NODE NAME ");
         Write_String (stdout,NodeName(T));
         printf ("\n");

   } /* end switch */
} /* end Expression */



Clabel ProcessNode (TreeNode T, Clabel CurrLabel)
{
   int Kid, Num;
   Clabel Label1, Label2, Label3,cascLabel;

  
   if (TraceSpecified)
   {
      fprintf (TraceFile, "<<< CODE GENERATOR >>> Processing Node ");
      Write_String (TraceFile, NodeName (T) );
      fprintf (TraceFile, " , Label is  ");
      Write_String (TraceFile, CurrLabel);
      fprintf (TraceFile, "\n");
   }

   switch (NodeName(T))
   {

      case ProgramNode :
	
	
	 CurrLabel = ProcessNode (Child(T,2),CurrLabel);			 
	 CurrLabel = ProcessNode (Child(T,3),CurrLabel);
	 CurrLabel = ProcessNode (Child(T,4),CurrLabel);
         CurrLabel = ProcessNode (Child(T,5),CurrLabel);

	  Label1 = MakeLabel();
	CodeGen1(GOTOOP, Label1, CurrLabel) ;

	Label2=ProcessNode (Child(T,6),NoLabel);
	
	CurrLabel= ProcessNode (Child(T,7),Label1);
         return (CurrLabel);


	case SubProgNode:	
	         for (Kid = 1; Kid <= NKids(T); Kid++)
       			ProcessNode (Child(T,Kid),CurrLabel);
	return CurrLabel;

      case FcnNode:
		OpenFrame();
		Decorate (Child(T,1),MakeAddress());
		IncrementFrameSize();
		Label2= MakeLabel();
		Decorate(T,Label2);
		CurrLabel=ProcessNode(Child(T,2), CurrLabel);
		CurrLabel=ProcessNode(Child(T,4),CurrLabel);
		CurrLabel=ProcessNode(Child(T,5),CurrLabel);
		CurrLabel=ProcessNode(Child(T,6),Label2);
		CurrLabel=ProcessNode(Child(T,7),CurrLabel);
		CodeGen1 (LLVOP,MakeStringOf(0),CurrLabel);
		CodeGen1 (RTNOP,MakeStringOf(1),NoLabel);
		CloseFrame();
		return NoLabel;

      case ProcedureNode:
		OpenFrame();
		Decorate (Child(T,1),MakeAddress());
		Label2= MakeLabel();
		Decorate(T,Label2);
		CurrLabel=ProcessNode(Child(T,2), CurrLabel);
		CurrLabel=ProcessNode(Child(T,3),CurrLabel);
		CurrLabel=ProcessNode(Child(T,4),CurrLabel);
		CurrLabel=ProcessNode(Child(T,5),Label2);
		CurrLabel=ProcessNode(Child(T,6),CurrLabel);
		CodeGen1 (RTNOP,MakeStringOf(1),CurrLabel);
		CloseFrame();
		return NoLabel;



      case ParamsNode:
		for (Kid = 1; Kid <= NKids(T); Kid++){
			int k = 1;	
			for (k = 1; k < NKids(Child(T,Kid )); k++){    
				
				Num = MakeAddress();
            			Decorate ( Child(Child(T,Kid),k), Num);		
				IncrementFrameSize();			
					
			}		
		}
	return CurrLabel;


 
	case ReturnNode:
		if(NKids(T)>0){
			Expression (Child(T,1), CurrLabel);			
			CodeGen1 (RTNOP,MakeStringOf(1),NoLabel);
			DecrementFrameSize();
			return NoLabel;
		}
		else{
			return CurrLabel;
		}

	case CallNode:
		
		Label2 = Decoration(Decoration(Child(Decoration(Child(T,1)),1)));
		for (Kid = 2; Kid <= NKids(T); Kid++){
			Expression (Child(T,Kid), NoLabel);	
		}
		CodeGen1 (CODEOP,Label2, NoLabel);		
		
		for (Kid = 2; Kid <= NKids(T); Kid++){

			DecrementFrameSize();
		}
		
		CodeGen1 (CALLOP,MakeStringOf(FrameSize-1), NoLabel);		
			
	return NoLabel;


      case TypesNode :

         for (Kid = 1; Kid <= NKids(T); Kid++)
            CurrLabel = ProcessNode (Child(T,Kid), CurrLabel);
         return (CurrLabel);

	case TypeNode :
		if(Rank(T)==2){
			CurrLabel = ProcessNode (Child(T,2), CurrLabel); 

		}		 
		return (CurrLabel);


	case LITNode :
		 for (Kid = 1; Kid <= NKids(T); Kid++)
		 {
			Decorate(Child(T,Kid),MakeStringOf(Kid-1));            

		 }
		return (CurrLabel);


      case DclnsNode :
         for (Kid = 1; Kid <= NKids(T); Kid++)
            CurrLabel = ProcessNode (Child(T,Kid), CurrLabel);
         if (NKids(T) > 0)
            return (NoLabel);
         else
            return (CurrLabel);

      case VarNode :
         for (Kid = 1; Kid < NKids(T); Kid++)
         {
            if (Kid != 1)
               CodeGen1 (LITOP,MakeStringOf(0),NoLabel);
            else
               CodeGen1 (LITOP,MakeStringOf(0),CurrLabel);
            Num = MakeAddress();
            Decorate ( Child(T,Kid), Num);
            IncrementFrameSize();
         }
         return (NoLabel);   



      case ConstsNode :
         
	for (Kid = 1; Kid <= NKids(T); Kid++)
            CurrLabel = ProcessNode (Child(T,Kid), CurrLabel);
        		
            return (CurrLabel);

      case ConstNode :
         
	if(NodeName(Child(T,2))== IntegerNode){
	 	Decorate(Child(T,1), (NodeName(Child(Child(T,2),1))));	
        
	}
	else if(NodeName(Child(T,2))== CharacterNode){
		
		Decorate(Child(T,1), MakeStringOf(Character(NodeName(Child(Child(T,2),1)),2)));
	        

	}	        
	else{
			
		int dec1 = Decoration(Child(T,2));
		int dec2 = Decoration(Child(dec1,1));

		if(NodeName(Child(dec2,2))== CharacterNode ){
			Decorate(Child(T,1),  MakeStringOf(Character(NodeName(Child(Child(dec2,2),1)),2)));			
				
		}
		else if(NodeName(Child(dec2,2))== IntegerNode ){
			
			Decorate(Child(T,1), NodeName(Child(Child(dec2,2),1)));
		}
		else{
			if(NodeName(Child(Child(T,2),1))== true){	
				Decorate(Child(T,1),  MakeStringOf(1));
			}
			else{	
				Decorate(Child(T,1),  MakeStringOf(0));
			}			
		}

	}

	return (CurrLabel);   
      

      case BlockNode :
         for (Kid = 1; Kid <= NKids(T); Kid++)
            CurrLabel = ProcessNode (Child(T,Kid), CurrLabel);
         return (CurrLabel); 


      case AssignNode :

       	Expression (Child(T,2), CurrLabel);
       	Reference (Child(T,1), LeftMode, NoLabel);
	return (NoLabel);


      case OutputNode :
        
	Expression (Child(T,1), CurrLabel);
	int d1,d2;	

	if(NodeName(Child(T,1)) ==OrdNode || NodeName(Child(T,1))== SuccNode || NodeName(Child(T,1)) ==PrecNode){
		d1 = Decoration(Child(Child(T,1),1));
		d2 = Decoration(Child(d1,1));	
	}
	else{
		d1 = Decoration(Child(T,1));
		d2 = Decoration(Child(d1,1));
	}

	/* For succ, pred,ord,chr*/
	if(NodeName(Child(T,1))==ChrNode){
		CodeGen1 (SOSOP, OSOUTPUTC, NoLabel);	
	}	
	
	/*For numbers and char*/
	else if(NodeName(Child(T,1))==IntegerNode){
	        CodeGen1 (SOSOP, OSOUTPUT, NoLabel);	
	}
	else if(NodeName(Child(T,1))==CharacterNode){
		CodeGen1 (SOSOP, OSOUTPUTC, NoLabel);
	}
	else if(NodeName(Child(T,1))==StringNode){
		
		TreeNode t1 = 	Child(Child(T,1),1);
		int length =StringLength(NodeName(t1));

		if(length!=0){
			for(Kid=2;Kid<length;Kid++){
				if(Kid==2)
					CodeGen1 (LITOP, MakeStringOf(Character(NodeName(t1),Kid)), CurrLabel);
				else
					CodeGen1 (LITOP, MakeStringOf(Character(NodeName(t1),Kid)), NoLabel);				
				CodeGen1 (SOSOP, OSOUTPUTC, NoLabel);			
			}
		}
	}

	/*For variables*/
	else if(NodeName(Child(Child(d2,NKids(d2)),1))== IntegerTNode){
					       
			CodeGen1 (SOSOP, OSOUTPUT, NoLabel);	
	}
	else if(NodeName(Child(Child(d2,NKids(d2)),1))== CharacterTNode){
			
		       CodeGen1 (SOSOP, OSOUTPUTC, NoLabel);	
	}
	else if(NodeName(Child(Child(d2,NKids(d2)),1))== BooleanTNode){
			CodeGen1 (SOSOP, OSOUTPUT, NoLabel);	
	}

	/* For constant*/
	else if(NodeName(Child(d2,2))== IntegerNode){
			CodeGen1 (SOSOP, OSOUTPUT, NoLabel);	
	}
	else if(NodeName(Child(d2,2))== CharacterNode){
		       CodeGen1 (SOSOP, OSOUTPUTC, NoLabel);	
	}
	else{
		int d3 = Decoration(Child(d2,2));
		int d4 = Decoration(Child(d3,1));

		    if(NodeName(Child(d4,2))== CharacterNode){
				
		            CodeGen1 (SOSOP, OSOUTPUTC, NoLabel);
          	    }
		    else if(NodeName(Child(d4,2))== IntegerNode){
			   CodeGen1 (SOSOP, OSOUTPUT, NoLabel);
          	    }
		    else{
		            CodeGen1 (SOSOP, OSOUTPUT, NoLabel);
		    }		    	    
 	}
         DecrementFrameSize();

         for (Kid = 2; Kid <= NKids(T); Kid++)
         {
		Expression (Child(T,Kid), NoLabel);
		    
		if(NodeName(Child(T,Kid)) ==OrdNode || NodeName(Child(T,Kid))== SuccNode || NodeName(Child(T,Kid)) ==PrecNode){
			d1 = Decoration(Child(Child(T,Kid),1));
			d2 = Decoration(Child(d1,1));	
		}
		else{
			d1 = Decoration(Child(T,Kid));
			d2 = Decoration(Child(d1,1));
		}

		if(NodeName(Child(T,Kid))==ChrNode){
			CodeGen1 (SOSOP, OSOUTPUTC, NoLabel);	
		}	

		else if(NodeName(Child(T,Kid))==IntegerNode){
			CodeGen1 (SOSOP, OSOUTPUT, NoLabel);	
		}
		else if(NodeName(Child(T,Kid))==CharacterNode){
			CodeGen1 (SOSOP, OSOUTPUTC, NoLabel);
		}
		else if(NodeName(Child(T,Kid))==StringNode){
	
			TreeNode t1 = 	Child(Child(T,Kid),1);
			int length =StringLength(NodeName(t1));
			int i;
			if(length!=0){
				for(i=2;i<length;i++){
					CodeGen1 (LITOP, MakeStringOf(Character(NodeName(t1),i)), NoLabel);
					CodeGen1 (SOSOP, OSOUTPUTC, NoLabel);			
				}
			}

		}
		else if(NodeName(Child(Child(d2,NKids(d2)),1))== IntegerTNode){
			       CodeGen1 (SOSOP, OSOUTPUT, NoLabel);	
		}
		else if(NodeName(Child(Child(d2,NKids(d2)),1))== CharacterTNode){
			       CodeGen1 (SOSOP, OSOUTPUTC, NoLabel);	
		}
		else if(NodeName(Child(Child(d2,NKids(d2)),1))== BooleanTNode){
			 CodeGen1 (SOSOP, OSOUTPUT, NoLabel);	
		}
		
		else if(NodeName(Child(d2,2))== IntegerNode){
		       CodeGen1 (SOSOP, OSOUTPUT, NoLabel);	
		}
		else if(NodeName(Child(d2,2))== CharacterNode){
		       CodeGen1 (SOSOP, OSOUTPUTC, NoLabel);	
		}
		else{
	
		    int d3 = Decoration(Child(d2,2));
		    int d4 = Decoration(Child(d3,1));

		    if(NodeName(Child(d4,2))== CharacterNode){
			    CodeGen1 (SOSOP, OSOUTPUTC, NoLabel);
          	    }
		    else if(NodeName(Child(d4,2))== IntegerNode){
			    CodeGen1 (SOSOP, OSOUTPUT, NoLabel);
          	    }
		    else{
			    CodeGen1 (SOSOP, OSOUTPUT, NoLabel);
		    }
			
		}
			DecrementFrameSize();
         }
         CodeGen1 (SOSOP, OSOUTPUTL, NoLabel);
         return (NoLabel);

	case ReadNode :
	
				
	d1 = Decoration(Child(T,1));
	d2 = Decoration(Child(d1,1));
		

	if(NodeName(Child(Child(d2,NKids(d2)),1))== IntegerTNode){
		       CodeGen1 (SOSOP, OSINPUT, CurrLabel);	
			IncrementFrameSize();
	}
	else if(NodeName(Child(Child(d2,NKids(d2)),1))== CharacterTNode){
		       CodeGen1 (SOSOP, OSINPUTC, CurrLabel);	
			IncrementFrameSize();
	}
	
	Reference (Child(T,1), LeftMode, NoLabel);

	
	 for (Kid = 2; Kid <= NKids(T); Kid++)
         {
		Expression (Child(T,Kid), NoLabel);
		
		int d1 = Decoration(Child(T,Kid));
		int d2 = Decoration(Child(d1,1));
		

		if(NodeName(Child(Child(d2,NKids(d2)),1))== IntegerTNode){
			       CodeGen1 (SOSOP, OSINPUT, NoLabel);	
		}
		else if(NodeName(Child(Child(d2,NKids(d2)),1))== CharacterTNode){
			       CodeGen1 (SOSOP, OSINPUTC, NoLabel);	
		}
		
		IncrementFrameSize();
		Reference (Child(T,Kid), LeftMode, NoLabel);
		
         }
         
        
	return(NoLabel);	


      case IfNode :
         Expression (Child(T,1), CurrLabel);
         Label1 = MakeLabel();
         Label2 = MakeLabel();
         Label3 = MakeLabel();
         CodeGen2 (CONDOP,Label1,Label2, NoLabel);
         DecrementFrameSize();
         CodeGen1 (GOTOOP, Label3, ProcessNode (Child(T,2), Label1) );
         if (Rank(T) == 3)
            CodeGen0 (NOP, ProcessNode (Child(T,3),Label2));
         else
            CodeGen0 (NOP, Label2);
         return (Label3);                


      case WhileNode :
         if (CurrLabel == NoLabel) 
            Label1 = MakeLabel();
         else 
            Label1 = CurrLabel;
         Label2 = MakeLabel();
         Label3 = MakeLabel();
         Expression (Child(T,1), Label1);
         CodeGen2 (CONDOP, Label2, Label3, NoLabel);
         DecrementFrameSize();
         CodeGen1 (GOTOOP, Label1, ProcessNode (Child(T,2), Label2) );
         return (Label3);


       case NullNode : return(CurrLabel);

	case RepeatNode :
	   if (CurrLabel == NoLabel) {
           	Label1 = MakeLabel();
		CurrLabel = Label1;
	   }
           else 
            	Label1 = CurrLabel;

	   Label2 = MakeLabel();

	   for (Kid = 1; Kid < NKids(T); Kid++)
	   {
            	CurrLabel= ProcessNode(Child(T,Kid), CurrLabel);
            	
            }
	 Expression (Child(T,NKids(T)), CurrLabel);
	 CodeGen2 (CONDOP, Label2, Label1, NoLabel);
	DecrementFrameSize();
	return (Label2);

	case LoopNode : 

	Label1 = MakeLabel();	
	Decorate (T, Label1);

	 
	 if (CurrLabel == NoLabel) {
   		Label2 = MakeLabel();
		CurrLabel = Label2;
	}
  	 else{ 
    		Label2 = CurrLabel;
	}
	
	   for (Kid = 1; Kid <= NKids(T); Kid++)
	   {
            	CurrLabel = ProcessNode(Child(T,Kid), CurrLabel);
            	
            }

	CodeGen1(GOTOOP, Label2, CurrLabel) ;

	return (Label1);

       case ExitNode : 
		Label1=Decoration(Decoration(T));
		CodeGen1(GOTOOP, Label1, CurrLabel);
		return (NoLabel); 
	
	case SwapNode : 
		Reference (Child(T,1), RightMode, NoLabel);
		Reference (Child(T,2), RightMode, NoLabel);
		Reference (Child(T,1), LeftMode, NoLabel);
		Reference (Child(T,2), LeftMode, NoLabel);
         return (NoLabel);

	case UptoForNode : 

		Label1 = MakeLabel();
	        Label2 = MakeLabel();
		Label3 = MakeLabel();
		cascLabel = MakeLabel();
		Expression (Child(T,3), CurrLabel);
		Expression (Child(T,2), NoLabel);
		Reference (Child(T,1),LeftMode,NoLabel);
		CodeGen0(DUPOP,Label1);
		IncrementFrameSize();
		Reference (Child(T,1),RightMode,NoLabel);
		CodeGen1(BOPOP,BGE, NoLabel);
		DecrementFrameSize();
		CodeGen2 (CONDOP, Label2, Label3, NoLabel);
		DecrementFrameSize();
		cascLabel= ProcessNode(Child(T,4), Label2);
		Reference (Child(T,1),RightMode,cascLabel);
		CodeGen1(UOPOP,USUCC, NoLabel);		
		Reference (Child(T,1),LeftMode,NoLabel);
		CodeGen1(GOTOOP, Label1, NoLabel);
		CodeGen1(POPOP,  MakeStringOf(1), Label3);
		DecrementFrameSize();
		CodeGen1(LITOP,  MakeStringOf(0), NoLabel);		
		IncrementFrameSize();	
		Reference (Child(T,1),LeftMode,NoLabel);

	return (NoLabel);


	case DownForNode : 

		 if (CurrLabel == NoLabel) {
           		Label1 = MakeLabel();		
	          }
           	else 
           	 	Label1 = CurrLabel;		

		Label2 = MakeLabel();
		Label3 = MakeLabel();
		cascLabel = MakeLabel();
		Expression (Child(T,3), CurrLabel);
		Expression (Child(T,2), NoLabel);
		Reference (Child(T,1),LeftMode,NoLabel);
		CodeGen0(DUPOP,Label1);
		DecrementFrameSize();
		Reference (Child(T,1),RightMode,NoLabel);
		CodeGen1(BOPOP,BLE, NoLabel);
		DecrementFrameSize();
		CodeGen2 (CONDOP, Label2, Label3, NoLabel);
		DecrementFrameSize();
		cascLabel = ProcessNode(Child(T,4), Label2);
		Reference (Child(T,1),RightMode,cascLabel);
		CodeGen1(UOPOP,UPRED, NoLabel);		
		Reference (Child(T,1),LeftMode,NoLabel);
		CodeGen1(GOTOOP, Label1, NoLabel);
		CodeGen1(POPOP,  MakeStringOf(1), Label3);
		DecrementFrameSize();
		CodeGen1(LITOP,  MakeStringOf(0), NoLabel);			
		IncrementFrameSize();
		Reference (Child(T,1),LeftMode,NoLabel);

	return (NoLabel);


	  case CaseNode :
	    Expression(Child(T,1),CurrLabel);
	    Label3 = NoLabel;
	    Clabel eLabel;
	    eLabel = MakeLabel();
	    for (Kid = 2; Kid <= NKids(T); Kid++){

		if(NodeName(Child(T,Kid)) != OtherwiseNode){

		    CodeGen0(DUPOP, Label3);
			IncrementFrameSize();
		    if( NodeName(Child(Child(T,Kid),1)) == SequenceNode){
		        CodeGen0(DUPOP, NoLabel);
			IncrementFrameSize();
		        Expression(Child(Child(Child(T,Kid),1),1),NoLabel);
		        CodeGen1(BOPOP,BGE, NoLabel);
			DecrementFrameSize();
		        CodeGen0(SWAPOP, NoLabel);
		        Expression(Child(Child(Child(T,Kid),1),2),NoLabel);
		        CodeGen1(BOPOP,BLE, NoLabel);
			DecrementFrameSize();
		        CodeGen1(BOPOP,BAND, NoLabel);
			DecrementFrameSize();

		    }
		    else{

			if(NodeName(Child(Child(Child(T,Kid),1),1))==TrueNode){
				CodeGen1(LITOP,MakeStringOf(1), NoLabel);				
			}
			else if(NodeName(Child(Child(Child(T,Kid),1),1))== FalseNode){
				CodeGen1(LITOP,MakeStringOf(0), NoLabel);
			}
			else{
		        	CodeGen1(LITOP,NodeName(Child(Child(Child(T,Kid),1),1)), NoLabel);
			}
			IncrementFrameSize();
		        CodeGen1(BOPOP,BEQ, NoLabel);
			DecrementFrameSize();
		    }
		    Label1 = MakeLabel();
		    Label2 = MakeLabel();           
		    CodeGen2(CONDOP,Label1,Label2, NoLabel);
			DecrementFrameSize();
		    CodeGen1(POPOP,MakeStringOf(1), Label1);
			DecrementFrameSize();
		    CurrLabel = ProcessNode(Child(Child(T,Kid),2),NoLabel);
		    CodeGen1(GOTOOP,eLabel, CurrLabel);
		    Label3 =Label2;

		    }
		}
		CodeGen1(POPOP,MakeStringOf(1), Label2);
	       DecrementFrameSize();
		if(NodeName(Child(T,NKids(T)))==OtherwiseNode){
		               
		    CurrLabel = ProcessNode(Child(Child(T,NKids(T)),1), NoLabel);
		   
		}
		return(eLabel);


      	default :
              ReportTreeErrorAt(T);
              printf ("<<< CODE GENERATOR >>> : UNKNOWN NODE NAME ");
              Write_String (stdout,NodeName(T));
              printf ("\n");

   } /* end switch */
}   /* end ProcessNode */
