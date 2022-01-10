(********************************************************************
 * COPYRIGHT --  B&R CHINA
 ********************************************************************
 * Program: recipe
 * File: recipe.typ
 * Author: lij
 * Created: March 06, 2013
 ********************************************************************
 * Local data types of program recipe
 ********************************************************************)

TYPE
	Text_Input_typ : 	STRUCT 
		CurrentName : STRING[16];
		TempName : STRING[16];
		Text : STRING[2];
		SourceChar : STRING[80];
		Add : USINT;
		Del : USINT;
		Flash : USINT;
		Enable : USINT;
		Layer : USINT;
		Reserve : USINT;
	END_STRUCT;
	recipeButton_typ : 	STRUCT 
		rename : USINT;
		save : USINT;
		open : USINT;
		clear : USINT;
		cancle : USINT;
		enter : USINT;
		Reserve2 : USINT;
		Reserve1 : USINT;
		former : USINT;
		next : USINT;
		lockFlag : UINT;
		lockListName : USINT;
	END_STRUCT;
	recipeName_typ : 	STRUCT 
		recipeName : ARRAY[0..49]OF STRING[16];
		recipeTime : ARRAY[0..49]OF STRING[20];
		recipeIndex : UINT;
	END_STRUCT;
END_TYPE
