#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct Author{
	struct Author *next;
	char authorName[50];
	char authorSurname[50];
	int authorId;
}Author;

typedef struct Student{
	char name[50];
	char surname[50];
	char studentId[9];
	int libraryScore;
	
}Student;

typedef struct Book{
	char title[50];
	char isbnNumber[14];
	int copyNumber;
	struct Book *next;
	struct BookCopies *p;
}Book;

typedef enum{
	ON_SHELF,
	BORROWED
}StatusShelf;
	
typedef union{
	char studentId[9];
	char statusOfBook[9];
}StatusData;

typedef struct BookCopies{
	char tagNumber[20];
	StatusShelf status;
	StatusData data;
}BookCopies;

typedef struct BooksAuthors{
	char isbn[14];
	int authorId;
}BooksAuthors;

typedef struct Borrow{
	char tagNumber[20];
	char studentId[9];
	int transactionType;
	char date[11];
}Borrow;

//function pointer
int isPenalized(Student s);
int isAll(Student s);
void listStudents(Student *student, int studentCount, int (*condition)(Student));
int matchById(Student s, char *p1, char *p2);
int matchByName(Student s, char *p1, char *p2);
void listStudentInfo(Student *student, int studentCount, Book *bookHead, int(*isMatch)(Student, char*, char*), char *p1, char *p2);
int isCopyOnShelf(BookCopies copy);
int isCopyBorrowed(BookCopies copy);
void listBookCopies(Book *bookHead, Student *student, int studentCount, int (*condition)(BookCopies));

//student applications
Student* addStudent(Student *student, int *studentCount);
Student* deleteStudent(Student *student, int *studentCount, char *studentId);
void editStudentInfo(Student *student, int studentCount, char *studentId);

//author applications
Author* addAuthor(Author *head);
Author* deleteAuthor(Author *head, int targetId, BooksAuthors *matchArray, int matchCount);
void editAuthorInfo(Author *head, int targetId);
void listAuthorInfo(Author *head, int targetId, BooksAuthors *matchArray, int matchCount, Book *bookHead);

//book applications
Book* addBook(Book *head);
Book* deleteBook(Book *head, char *targetIsbn);
void editBookInfo(Book *head, char *targetIsbn);
void listBookInfo(Book *head, char *targetIsbn);
void listBookOnShelf(Book *head);
void listOverdueBooks(Book *bookHead, Student *student, int studentCount, Borrow *logs, int borrowCount, char *currentDate);
void editBookAuthorMatch(BooksAuthors *matchArray, int matchCount, char *targetIsbn);

//borrowing and date applications
void saveBorrowsToCSV(Borrow *logs, int borrowCount);
Borrow* loadBorrowsFromCSV(int *borrowCount);
Borrow* addTransaction(Borrow *logs, int *borrowCount, char *tag, char *studentId, int type, char *date);
Borrow* borrowBook(Book *bookHead, Student *student, int studentCount, Borrow *logs, int *borrowCount, char *targetTag, char *targetId, char *currentDate);
Borrow* returnBook(Book *bookHead, Student *student, int studentCount, Borrow *logs, int *borrowCount, char *targetTag, char *currentDate);
int calculateDateDifference(char *borrowDate, char *returnDate);

//file applications
void saveStuToCSV(Student *student, int studentCount);
Student* loadStuFromCSV(Student *student, int *studentCount);
void saveAuthorToCSV(Author *head);
Author* loadAuthorFromCSV(Author *head);
void saveBookToCSV(Book *head);
Book* loadBookFromCSV(Book *head);
void saveBooksAuthorsToCSV(BooksAuthors *matchArray, int matchCount);
BooksAuthors* loadBooksAuthorsFromCSV(int *matchCount);
BooksAuthors* matchBookAuthor(BooksAuthors *matchArray, int *matchCount, Book *bookHead, Author *authorHead);

//freeing applications
void freeAll(Student *student, Author *author, Book *book, BooksAuthors *matchArray, Borrow* borrowLogs);

int main(){
	int mode = 0, studentCount =0, mode2, matchCount=0, borrowCount=0;
	Student *student = NULL;
	Author *author = NULL;
	Book *book = NULL;
	BooksAuthors *matchArray= NULL;
	Borrow *borrowLogs= NULL;
	
	printf("--Welcome to the library system--\n");
	
	student= loadStuFromCSV(student, &studentCount);
	author= loadAuthorFromCSV(author);
	book= loadBookFromCSV(book);
	matchArray= loadBooksAuthorsFromCSV(&matchCount);
	borrowLogs= loadBorrowsFromCSV(&borrowCount);

	while(mode!=15){
		
		printf("\n1- Add/Delete/Edit a student\n");
		printf("2- List student information\n");
		printf("3- List the students who did not return borrowed books\n");
		printf("4- List all the students that has detenion\n");
		printf("5- List all of the students\n");
		printf("6- Borrow/Return a book\n");
		printf("-----------------------------------------------------------\n");
		printf("7- Add/Delete/Edit a book\n");
		printf("8- List book information\n");
		printf("9- List the books on the shelf\n");
		printf("10- List the books that are not returned on time\n");
		printf("11- Match a book with an author\n");
		printf("12- Edit author of a book\n");
		printf("-----------------------------------------------------------\n");
		printf("13- Add/Delete/Edit an author\n");
		printf("14- List author information\n");
		printf("15- Exit\n");
		printf("-----------------------------------------------------------\n");
		printf("Please enter your desired operation:\n");
		scanf(" %d", &mode);
		
		if(mode==1){
			char targetId[9];
			printf("1- Add\n");
			printf("2- Delete\n");
			printf("3- Edit\n");
			printf("Please enter your desired operation:\n");
			scanf(" %d", &mode2);
			if(mode2==1) student= addStudent(student, &studentCount);
			else if(mode2==2){
				printf("Please enter the student id:\n");
				scanf(" %s", targetId);
				student= deleteStudent(student, &studentCount, targetId);
			}
			else if(mode2==3){
				printf("Please enter the student id:\n");
				scanf(" %s", targetId);
				editStudentInfo(student, studentCount, targetId);
			}
			else printf("Please enter one of the numbers on the screen\n");
		}
		else if(mode==2){
			char targetId[9]= "";
			char targetName[50]= "";
			char targetSurname[50]="";
			
			printf("1- Search by Student ID\n");
			printf("2- Search by Name and Surname\n");
			printf("Please enter your desired operation:\n");
			scanf(" %d", &mode2);
			if(mode2==1){
				printf("Please enter student id:\n");
				scanf(" %s", targetId);
				listStudentInfo(student, studentCount, book, matchById, targetId, "");
			}
			else if(mode2==2){
				printf("Please enter the student name:\n");
				scanf(" %[^\n]", targetName);
				printf("Please enter the student surname:\n");
				scanf(" %s", targetSurname);
				listStudentInfo(student, studentCount, book, matchByName, targetName, targetSurname);
			}
			else printf("Please enter one of the numbers on the screen");
		}
		else if(mode==3) listBookCopies(book, student, studentCount, isCopyBorrowed);
		else if(mode==4) listStudents(student, studentCount, isPenalized);
		else if(mode==5) listStudents(student, studentCount, isAll);
		else if(mode==6){
			char targetTag[20];
			char targetId[9];
			char currentDate[11];
			printf("1- Borrow a book\n");
			printf("2- Return a book\n");
			printf("Please enter your desired operation:\n");
			scanf(" %d", &mode2);
			if(mode2==1){
				printf("Please enter the tag number of the book copy:\n");
				scanf( "%s", targetTag);
				printf("Please enter the Student ID:\n");
				scanf(" %s", targetId);
				printf("Please enter today's date (DD.MM.YY):\n");
				scanf(" %s", currentDate);
				borrowLogs = borrowBook(book, student, studentCount, borrowLogs, &borrowCount, targetTag, targetId, currentDate);
			}
			else if(mode2==2){
				printf("Please enter the tag number of the book copy:\n");
				scanf(" %s", targetTag);
				printf("Please enter today's date (DD.MM.YY):\n");
				scanf(" %s", currentDate);
				borrowLogs = returnBook(book, student, studentCount, borrowLogs, &borrowCount, targetTag, currentDate);
			}
			else printf("Please enter one of the numbers on the screen\n");
		}
		else if(mode==7){
			char targetIsbn[9];
			printf("1- Add\n");
			printf("2- Delete\n");
			printf("3- Edit\n");
			printf("Please enter your desired operation:\n");
			scanf(" %d", &mode2);
			if(mode2==1) book= addBook(book);
			else if(mode2==2){
				printf("Please enter the book isbn:\n");
				scanf(" %s", targetIsbn);
				book= deleteBook(book, targetIsbn);
			}
			else if(mode2==3){
				printf("Please enter the book isbn:\n");
				scanf(" %s", targetIsbn);
				editBookInfo(book, targetIsbn);
			}
			else printf("Please enter one of the numbers on the screen\n");
		}
		else if(mode==8){
			char targetName[20];
			printf("Please enter name of the book:\n");
			scanf(" %[^\n]", targetName);
			listBookInfo(book, targetName);
		}
		else if(mode==9){
			listBookCopies(book, student, studentCount, isCopyOnShelf);
		}
		else if(mode==10){
			char currentDate[11];
			printf("Please enter today's date (DD.MM.YY):\n");
			scanf(" %s", currentDate);
			listOverdueBooks(book, student, studentCount, borrowLogs, borrowCount, currentDate);
		}
		else if(mode==11){ matchArray = matchBookAuthor(matchArray, &matchCount, book, author);
		}
		else if(mode==12){
			char targetIsbn[14];
			printf("Please enter the ISBN of the book:\n");
			scanf(" %s", targetIsbn);
			editBookAuthorMatch(matchArray, matchCount, targetIsbn);
		}
		else if(mode==13){
			int targetId;
			printf("1- Add\n");
			printf("2- Delete\n");
			printf("3- Edit\n");
			printf("Please enter your desired operation:\n");
			scanf(" %d", &mode2);
			if(mode2==1) author= addAuthor(author);
			else if(mode2==2){
				printf("Please enter the author id:\n");
				scanf(" %d", &targetId);
				author = deleteAuthor(author, targetId, matchArray, matchCount);
			}
			else if(mode2==3){
				printf("Please enter the author id:\n");
				scanf(" %d", &targetId);
				editAuthorInfo(author, targetId);
			}
			else printf("Please enter one of the numbers on the screen\n");
		}
		else if(mode==14){
			int targetId;
			printf("Please enter the author id:\n");
			scanf(" %d", &targetId);
			listAuthorInfo(author, targetId, matchArray, matchCount, book);
		}
	}
	freeAll(student, author, book, matchArray, borrowLogs);
	return 0;
}

//FILE FUNCTIONS
void saveStuToCSV(Student *student, int studentCount){
	int i;
	FILE *file = fopen("Students.csv", "w");
	if(file==NULL){
		printf("Cannot open the file.\n");
		return;
	}
	
	for(i=0; i<studentCount; i++){
		fprintf(file, "%s,%s,%s,%d\n",
		student[i].name,
		student[i].surname,
		student[i].studentId,
		student[i].libraryScore);
	}
	
	fclose(file);
}

Student* loadStuFromCSV(Student *student, int *studentCount){
	FILE *file = fopen("Students.csv", "r");
	if(file==NULL) return student;
	
	char line[200];
	int index;
	while(fgets(line, sizeof(line), file)!= NULL){
		(*studentCount)++;
		student = (Student*)realloc(student, (*studentCount)*sizeof(Student));
		if(student==NULL){
			printf("Cannot allocate the memory.\n");
			return student;
		}
		index=(*studentCount)-1;
		
		char *piece =strtok(line, ",");
		strcpy(student[index].name, piece);
	
		piece=strtok(NULL, ",");
		strcpy(student[index].surname, piece);
		
		piece=strtok(NULL, ",");
		strcpy(student[index].studentId, piece);
		
		piece=strtok(NULL, ",");
		student[index].libraryScore= atoi(piece);
	}
	fclose(file);
	return student;
}

void saveAuthorToCSV(Author *head){
	FILE *file= fopen("authors.csv", "w");
	if(file==NULL){
		printf("Cannot open the file.\n");
		return;
	}
	
	Author *temp=head;
	while(temp!=NULL){
		fprintf(file, "%d,%s,%s\n",
			temp->authorId,
			temp->authorName,
			temp->authorSurname);
		temp= temp->next;
	}
	fclose(file);
}

Author* loadAuthorFromCSV(Author *head){
	FILE *file= fopen("authors.csv", "r");
	if(file==NULL) return head;
	
	char line[200];
	while(fgets(line, sizeof(line), file) != NULL){
		Author *author;
		author=(Author*)malloc(sizeof(Author));
		char *piece=strtok(line, ",");
		author->authorId = atoi(piece);
		
		piece=strtok(NULL, ",");
		strcpy(author->authorName, piece);
		
		piece=strtok(NULL, ",\n");
		strcpy(author->authorSurname, piece);
		author->next=NULL;
		
		if(head==NULL){
			head = author;
		}
		else{
			Author *temp=head;
			while(temp->next!=NULL){
				temp =temp->next;
			}
			temp->next=author;
		}
	}
	return head;
}

void saveBookToCSV(Book *head){
	int i;
	FILE *file= fopen("Books.csv", "w");
	if(file==NULL){
		printf("Cannot open the file.\n");
		return;
	}
	
	Book *temp= head;
	while(temp!= NULL){
		fprintf(file, "%s,%s,%d\n", temp->title, temp->isbnNumber, temp->copyNumber);
		for(i=0; i<temp->copyNumber; i++){
			char info[15];
			if(temp->p[i].status== ON_SHELF){
				strcpy(info, temp->p[i].data.statusOfBook);
			}
			else{
				strcpy(info, temp->p[i].data.studentId);
			}
			
			fprintf(file, "%s,%d,%s\n", temp->p[i].tagNumber, temp->p[i].status, info);
		}
		temp= temp->next;
	}
	fclose(file);
}

Book* loadBookFromCSV(Book *head){
	int isError=0, isValid=1, i;
	FILE *file=fopen("Books.csv", "r");
	if(file==NULL){
		printf("Cannot open the file.\n");
		return head;
	}
	char line[300];
	Book *last=NULL;
	
	while(fgets(line, sizeof(line), file)!= NULL &&isError==0){
		Book *newBook;
		newBook=(Book*)malloc(sizeof(Book));
		if(newBook==NULL){
			printf("Cannot allocate the memory.\n");
			isError=1;
		}
		else{
			isValid=1;
			char *piece= strtok(line, ",");
			if(piece==NULL){
				free(newBook);
				isValid=0;
			}
			if(isValid==1){
				strcpy(newBook->title, piece);
				piece= strtok(NULL, ",");
				strcpy(newBook->isbnNumber, piece);
				piece= strtok(NULL, ",\n");
				newBook->copyNumber= atoi(piece);
				
				newBook->p=(BookCopies*)malloc(newBook->copyNumber*sizeof(BookCopies));
				if(newBook->p==NULL){
					free(newBook);
					printf("Cannot allocate the memory.\n");
					isError=1;
					isValid=0;
				}
				
				if(isValid==1){
					newBook->next=NULL;
					
					for(i=0; i<newBook->copyNumber; i++){
						if(fgets(line, sizeof(line),file)!=NULL){
							char *copyPiece= strtok(line, ",");
							strcpy(newBook->p[i].tagNumber, copyPiece);
							
							copyPiece= strtok(NULL, ",");
							newBook->p[i].status= atoi(copyPiece);
							copyPiece=strtok(NULL, ",\n");
							if(newBook->p[i].status==ON_SHELF) strcpy(newBook->p[i].data.statusOfBook, copyPiece);
							else strcpy(newBook->p[i].data.studentId, copyPiece);
							
						}
					}
					if(head==NULL){
						head= newBook;
						last=newBook;
					}
					else{
						last->next= newBook;
						last= newBook;
					}
					
				}
			}
		}
	}
	fclose(file);
	return head;
}

void saveBooksAuthorsToCSV(BooksAuthors *matchArray, int matchCount){
	int i;
	FILE *file= fopen("BookAuthor.csv", "w");
	if(file==NULL){
		printf("Cannot open the file.\n");
		return;
	}
	for(i=0; i<matchCount; i++){
		fprintf(file, "%s,%d\n", matchArray[i].isbn, matchArray[i].authorId);
	}
	fclose(file);
}

BooksAuthors* loadBooksAuthorsFromCSV(int *matchCount){
	int i=0;
	FILE *file= fopen("BookAuthor.csv", "r");
	if(file==NULL){
		*matchCount= 0;
		return NULL;
	}
	char line[100];
	int lines=0;
	while(fgets(line, sizeof(line), file) != NULL){
		lines++;
	}
	
	*matchCount= lines;
	fclose(file);
	BooksAuthors *matchArray= NULL;
	
	if(lines>0){
		matchArray= (BooksAuthors*)malloc(lines*sizeof(BooksAuthors));
		file= fopen("BookAuthor.csv", "r");
		if(file==NULL){
			return matchArray;
		}
		while(fgets(line, sizeof(line),file)!= NULL && i<lines){
			char *piece= strtok(line, ",");
			if(piece!=NULL){
				strcpy(matchArray[i].isbn, piece);
				
				piece= strtok(NULL, ",\n");
				if(piece!=NULL){
					matchArray[i].authorId= atoi(piece);
				}
			}
			i++;
		}
		fclose(file);
	}
	return matchArray;
}

//STUDENT FUNCTIONS

Student* addStudent(Student *student, int *studentCount){
	(*studentCount)++;
	
	student= (Student*)realloc(student, (*studentCount)*sizeof(Student));
	if(student==NULL){
		printf("Could not allocate the memory.\n");
		exit(1);
	}
	
	int index= (*studentCount)-1;
	printf("Please enter the name of the student:\n");
	scanf(" %s", student[index].name);
	printf("Please enter the surname of the student:\n");
	scanf(" %s", student[index].surname);
	printf("Please enter the student id of the student:\n");
	scanf(" %s", student[index].studentId);
	student[index].libraryScore = 100;
	saveStuToCSV(student, *studentCount);
	return student;
}

Student* deleteStudent(Student* student, int *studentCount, char *targetId){
	int foundIndex=-1, i=0, j;
	
	while(foundIndex==-1 && i<*studentCount){
		if(strcmp(student[i].studentId, targetId)==0) foundIndex=i;
		i++;
	}
	
	if(foundIndex==-1){
		printf("Student not found.\n");
		return student;
	}
	
	for(j=foundIndex; j<(*studentCount)-1; j++){
		student[j]=student[j+1];
	}
	(*studentCount)--;
	if(*studentCount > 0){
		student=(Student*)realloc(student, (*studentCount)*sizeof(Student));
	}
	else{
		free(student);
		student= NULL;
	}
	saveStuToCSV(student, *studentCount);
	return student;
}

void editStudentInfo(Student *student, int studentCount, char *targetId){
	int foundIndex=-1, i=0;
	
	while(foundIndex==-1 && i<studentCount){
		if(strcmp(student[i].studentId, targetId)==0) foundIndex=i;
		i++;
	}
	
	if(foundIndex==-1){
		printf("Student not found.\n");
		return;
	}
	
	printf("Please enter the new name:\n");
	scanf(" %s", student[foundIndex].name);
	printf("Please enter the new surname:\n");
	scanf(" %s", student[foundIndex].surname);
	printf("Student information edited successfully.\n");
	saveStuToCSV(student, studentCount);
	return;
}

int matchById(Student s, char *p1, char *p2){
	if(strcmp(s.studentId, p1)==0) return 1;
	return 0;
}

int matchByName(Student s, char *p1, char *p2){
	if(strcmp(s.name, p1)==0 && strcmp(s.surname, p2)==0) return 1;
	return 0;
}
void listStudentInfo(Student *student, int studentCount, Book *bookHead, int(*isMatch)(Student, char*, char*), char *p1, char *p2){
	int i=0, isFound=0, index=0;
	
	while(i< studentCount && isFound==0){
		if(isMatch(student[i], p1, p2)==1){
			isFound= 1;
			index= i;
		}
		i++;
	}
	if(isFound==1){
		int hasBook=0, j=0;
		Book *bTemp = bookHead;
		
		printf("------------------------------------\n");
		printf("------------STUDENT INFO------------\n\n");
		printf("ID    : %s\n", student[index].studentId);
		printf("Name  : %s %s\n", student[index].name, student[index].surname);
		printf("Score : %d\n", student[index].libraryScore);
		printf("------------------------------------\n");
		printf("------------BORROWED BOOKS----------\n");
		
		while(bTemp !=NULL){
			j=0;
			while(j< bTemp->copyNumber){
				if(bTemp->p[i].status == BORROWED && strcmp(bTemp->p[i].data.studentId, student[i].studentId)==0){
					printf(" -Title: %-20s | Tag Number: %s\n", bTemp->title, bTemp->p[i].tagNumber);
					hasBook=1;
				}
				j++;
			}
			bTemp= bTemp->next;
		}
		if(hasBook==0) printf("This student currently has no borrowed books.\n");
		printf("------------------------------------\n\n");
	}
	else printf("Student not found.\n");
}

int isPenalized(Student s){
	if(s.libraryScore < 100) return 1;
	return 0;
}

int isAll(Student s){
	return 1;
}

void listStudents(Student *student, int studentCount, int (*condition)(Student)){
	int i, isFound;
	if(studentCount ==0){
		printf("Student not found.\n");
	}
	else{
		printf("--------Student List--------\n");
		for(i=0; i<studentCount; i++){
			if(condition(student[i]) == 1){
				printf(" %d. Student Id: %s |Name: %s |Surname: %s |Library Score: %d\n",
				i+1,
				student[i].studentId,
				student[i].name,
				student[i].surname,
				student[i].libraryScore);
			}
		}
		printf("------------------------------------\n\n");
	}
}


//AUTHOR FUNCTIONS

Author* addAuthor(Author *head){
	Author *author;
	author = (Author*)malloc(sizeof(Author));
	if(author==NULL){
		printf("Cannot allocate memory.\n");
		exit(1);
	}
	
	printf("Please enter the name of the author:\n");
	scanf(" %[^\n]", author->authorName);
	printf("Please enter the surname of the author:\n");
	scanf(" %s", author->authorSurname);
	
	author->next=NULL;
	if(head==NULL){
		author->authorId=1;
		head= author;
	}
	else{
		Author *temp= head;
		while(temp->next!= NULL){
			temp= temp->next;
		}
		author->authorId= temp->authorId + 1;
		temp->next= author;
	}
	printf("Author created. Author ID: %d\n", author->authorId);
	saveAuthorToCSV(head);
	return head;
}

Author* deleteAuthor(Author *head, int targetId, BooksAuthors *matchArray, int matchCount){
	if(head==NULL) return NULL;
	int isDeleted=0;
	if(head->authorId==targetId){
		Author *temp=head;
		head=head->next;
		free(temp);
		isDeleted=1;
	}
	else{
		Author *temp= head;
		while(temp->next!=NULL && temp->next->authorId!= targetId){
			temp= temp->next;
		}
		if(temp->next!=NULL){
			Author *deleted= temp->next;
			temp->next= temp->next->next;
			free(deleted);
			isDeleted=1;
		}
		else{
			printf("Author not found.\n");
		}
	}
	int i;
	if(isDeleted==1){
		saveAuthorToCSV(head);
		
		for(i=0; i< matchCount; i++){
			if(matchArray[i].authorId== targetId){
				matchArray[i].authorId= -1;
			}
		}
		saveBooksAuthorsToCSV(matchArray, matchCount);
	}

	return head;
}

void editAuthorInfo(Author *head, int targetId){
	Author *temp= head;
	int isEditDone=0;
	while(temp!=NULL && isEditDone==0){
		if(temp->authorId==targetId){
			printf("Please enter the name of the author:\n");
			scanf(" %[^\n]", temp->authorName);
			printf("Please enter the surname of the author:\n");
			scanf(" %s", temp->authorSurname);
			isEditDone =1;
		}
		temp= temp->next;
	}
	
	if(isEditDone==0) printf("Author not found.\n");
	saveAuthorToCSV(head);
	return;
}

void listAuthorInfo(Author *head, int targetId, BooksAuthors *matchArray, int matchCount, Book *bookHead){
	int isFound=0;
	Author *temp = head;
	
	while(temp != NULL && isFound==0){
		if(temp->authorId==targetId){
			isFound=1;
			printf("\nAuthor Info---------------------------\n");
			printf("ID: %d | Name: %s %s\n", temp->authorId, temp->authorName, temp->authorSurname);
			printf("Registered Books:\n");
			
			int hasBooks=0, i;
			for(i=0; i<matchCount; i++){
				if(matchArray[i].authorId==targetId){
					Book *bTemp= bookHead;
					int bookFound=0;
					while(bTemp!= NULL && bookFound==0){
						if(strcmp(bTemp->isbnNumber, matchArray[i].isbn)==0){
							printf("Title: %-20s | ISBN: %s\n", bTemp->title, bTemp->isbnNumber);
							hasBooks=1;
							bookFound=1;
						}
						bTemp= bTemp->next;
					}
				}
			}
			if(hasBooks==0){
				printf("No registered books found.\n");
			}
			printf("------------------------------------\n\n");
		}
		temp= temp->next;
	}
	if(isFound==0) printf("Author not found.\n");
}

//BOOK FUNCTIONS

Book* addBook(Book* head){
	int i, isFound=0;
	Book *book;
	book = (Book*)malloc(sizeof(Book));
	if(book==NULL){
		printf("Cannot allocate memory.\n");
		exit(1);
	}
	
	printf("Please enter the title of the book:\n");
	scanf(" %[^\n]", book->title);
	printf("Please enter the isbn number of the book:\n");
	scanf(" %s", book->isbnNumber);
	printf("Please enter the number of copies:\n");
	scanf(" %d", &book->copyNumber);
	
	book->p= (BookCopies*)malloc(book->copyNumber*sizeof(BookCopies));
	if(book->p==NULL){
		printf("Cannot allocate memory.\n");
		exit(1);
	}
	
	for(i=0; i<book->copyNumber; i++){
		sprintf(book->p[i].tagNumber, "%s_%d", book->isbnNumber, i+1);
		book->p[i].status= ON_SHELF;
		strcpy(book->p[i].data.statusOfBook, "ON_SHELF");
	}
	
	book->next=NULL;
	if(head==NULL){
		head= book;
	}
	else{
		Book *temp= head, *prev= NULL;
		while(temp!= NULL && isFound==0){
			if(strcmp(temp->title, book->title)>0 || (strcmp(temp->title, book->title)==0 && strcmp(temp->isbnNumber, book->isbnNumber)>0)){
				isFound=1;
			}
			else{
				prev= temp;
				temp=temp->next;
			}
		}
		if(prev==NULL){
			book->next= head;
			head= book;
		}
		else{
			book->next= temp;
			prev->next= book;
		}
	}
	saveBookToCSV(head);
	return head;
}

Book* deleteBook(Book *head, char *targetIsbn){
	int isFound=0;
	if(head==NULL) return NULL;
	
	Book *temp= head;
	Book *prev= NULL;
	
	while(temp!=NULL && isFound==0){
		if(strcmp(temp->isbnNumber, targetIsbn)==0){
			isFound=1;
		}
		else{
			prev= temp;
			temp= temp->next;
		}
	}
	if(isFound==0) printf("Book not found.\n");
	else{
		if(prev==NULL){
			head= temp->next;
		}
		else{
			prev->next=temp->next;
		}
		if(temp->p!=NULL){
			free(temp->p);
		}
		free(temp);
	}
	saveBookToCSV(head);
	return head;
}

void editBookInfo(Book *head, char *targetIsbn){
	int isFound=0;
	Book *temp= head;
	
	while(temp!=NULL && isFound==0){
		if(strcmp(temp->isbnNumber, targetIsbn)==0){
			isFound=1;
		}
		else{
			temp= temp->next;
		}
	}
	if(isFound==0) printf("Book not found.\n");
	else{
		printf("Please enter the new title:\n");
		scanf(" %[^\n]", temp->title);
	}
	saveBookToCSV(head);
}

void listBookInfo(Book *head, char *targetName){
	int isFound=0, i;
	Book *temp= head;
	
	printf("\n------------------------------------\n");
	while(temp!=NULL && isFound==0){
		
		if(strcmp(temp->title, targetName)==0){
			isFound=1;
			printf("TITLE: %s\n", temp->title);
			printf("ISBN: %s | Total Copies: %d\n", temp->isbnNumber, temp->copyNumber);
			printf("------------------------------------\n");
			
			for(i=0; i<temp->copyNumber; i++){
				printf("-> Tag Number: %-15s | Status: ", temp->p[i].tagNumber);
				if(temp->p[i].status==ON_SHELF) printf("ON SHELF | Info: %s\n", temp->p[i].data.statusOfBook);
				else printf("BORROWED | Student ID: %s\n", temp->p[i].data.studentId);
			}
			printf("\n------------------------------------\n");
		}
		else{
			temp=temp->next;
		}
	}
	if(isFound==0){
		printf("Book not found.\n");
	}
}

void listBookOnShelf(Book *head){
	int isFound=0, i;
	Book *temp= head;
	
	printf("\n------------------------------------\n");
	printf("BOOKS ON THE SHELF:\n");
	while(temp!= NULL){
		for(i=0; i<temp->copyNumber; i++){
			if(temp->p[i].status==ON_SHELF){
				printf("Title: %-25s | Tag Number: %s\n", temp->title, temp->p[i].tagNumber);
				isFound=1;
			}
		}
		temp=temp->next;
	}
	
	if(isFound==0){
		printf("No books on shelf.\n");
	}
	printf("\n------------------------------------\n");
}

void listOverdueBooks(Book *bookHead, Student *student, int studentCount, Borrow *logs, int borrowCount, char *currentDate){
	int isFound=0;
	Book *temp= bookHead;
	
	printf("\n------------------------------------\n");
	printf("             OVERDUE BOOKS            \n");
	printf("------------------------------------\n");
	while(temp != NULL){
		int i=0;
		while(i< temp->copyNumber){
			if(temp->p[i].status ==BORROWED){
				char targetTag[20];
				strcpy(targetTag, temp->p[i].tagNumber);
				
				char borrowDate[11] = "";
                int index = borrowCount - 1; 
                int dateFound = 0;
                
                while(index >= 0 && dateFound==0){
                	if(strcmp(logs[index].tagNumber, targetTag)==0 && logs[index].transactionType ==0){
                		strcpy(borrowDate, logs[index].date);
                		dateFound=1;
					}
					index--;
				}
				if(dateFound==1){
					int difference= calculateDateDifference(borrowDate, currentDate);
					if(difference > 15){
						isFound=1;
						char borrowedId[9];
						strcpy(borrowedId, temp->p[i].data.studentId);
						
						int stuIndex =-1, j=0;
						while(stuIndex== -1 && j< studentCount){
							if(strcmp(student[j].studentId, borrowedId)==0) stuIndex= j;
							j++;
						}
						printf("Tag: %-15s | Book: %-20s\n", targetTag, temp->title);
						printf("   -> Borrow date: %s | Overdue by %d days\n", borrowDate, difference-15);
						if(stuIndex !=-1) printf("   -> Student: %s %s | ID: %s\n", student[stuIndex].name, student[stuIndex].surname, student[stuIndex].studentId);
						printf("------------------------------------\n");
					}
				}
			}
			i++;
		}
		temp= temp->next;
	}
	if(isFound==0) printf("No overdue books at the moment.\n");
	printf("------------------------------------\n");
}

BooksAuthors* matchBookAuthor(BooksAuthors *matchArray, int *matchCount, Book *bookHead, Author *authorHead){
	char targetIsbn[14];
	int i=0, targetAuthorId, isBookFound=0, isAuthorFound=0, isMatched=0;
	
	printf("Please enter the isbn of the book:\n");
	scanf(" %s", targetIsbn);
	printf("Please enter the Author ID:\n");
	scanf(" %d", &targetAuthorId);
	
	Book *bTemp=bookHead;
	while(bTemp!= NULL && isBookFound==0){
		if(strcmp(bTemp->isbnNumber, targetIsbn)==0) isBookFound=1;
		else bTemp= bTemp->next;
	}
	Author *aTemp= authorHead;
	while(aTemp!= NULL && isAuthorFound==0){
		if(aTemp->authorId==targetAuthorId) isAuthorFound=1;
		else aTemp= aTemp->next;
	}
	
	if(isBookFound==0) printf("Unregistered book, match failed.\n");
	else if(isAuthorFound==0) printf("Unregistered author, match failed.\n");
	else{
		while(i< *matchCount && isMatched==0){
			if(strcmp(matchArray[i].isbn, targetIsbn)==0 && matchArray[i].authorId==targetAuthorId) isMatched=1;
			i++;
		}
		
		if(isMatched==1) printf("This book and author are already matched.\n");
		else{
			(*matchCount)++;
			if(*matchCount==1) matchArray= (BooksAuthors*)malloc(sizeof(BooksAuthors));
			else matchArray=(BooksAuthors*)realloc(matchArray, (*matchCount)*sizeof(BooksAuthors));
			
			int index=(*matchCount)-1;
			strcpy(matchArray[index].isbn, targetIsbn);
			matchArray[index].authorId= targetAuthorId;
			
			saveBooksAuthorsToCSV(matchArray, *matchCount);
		}
	}
	return matchArray;
}

void editBookAuthorMatch(BooksAuthors *matchArray, int matchCount, char *targetIsbn){
	int i=0, isFound=0;
	
	while(i<matchCount && isFound ==0){
		if(strcmp(matchArray[i].isbn, targetIsbn)==0){
			isFound=1;
			
			printf("Please enter the new Author ID:\n");
			scanf(" %d", &matchArray[i].authorId);
			saveBooksAuthorsToCSV(matchArray, matchCount);
		}
		i++;
	}
	if(isFound==0) printf("Author with id '%s' not found.\n", targetIsbn);
}

int isCopyOnShelf(BookCopies copy){
	if(copy.status== ON_SHELF) return 1;
	return 0;
}

int isCopyBorrowed(BookCopies copy){
	if(copy.status== BORROWED) return 1;
	return 0;
}

void listBookCopies(Book *bookHead, Student *student, int studentCount, int (*condition)(BookCopies)){
	Book *temp= bookHead;
	int isFound=0, i;
	
	printf("-------------BOOK COPIES------------\n");
	while(temp!=NULL){
		i=0;
		while(i< temp->copyNumber){
			if(condition(temp->p[i])==1){
				isFound=1;
				printf("Book: %-20s | Tag: %s | Status: ",temp->title, temp->p[i].tagNumber);
				
				if(temp->p[i].status==ON_SHELF) printf("ON SHELF\n");
				else{
					int stuIndex= -1, j=0;
					while(stuIndex==-1 && j< studentCount){
						if(strcmp(student[j].studentId, temp->p[i].data.studentId) ==0) stuIndex= j;
						j++;
					}
					if(stuIndex != -1) printf("BORROWED by %s %s with the ID: %s\n", student[stuIndex].name, student[stuIndex].surname, student[stuIndex].studentId);
				}
			}
			i++;
		}
		temp= temp->next;
	}
	if(isFound==0) printf("No book copies found eligible.\n");
	printf("------------------------------------\n");
	
}

//BORROW AND DATE FUNCTIONS

int calculateDateDifference(char *borrowDate, char *returnDate){
	int i, d1, m1, y1, d2, m2, y2;
	long int total1, total2;
	int monthDays[12]= {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
	char tempBorrow[15];
	char tempReturn[15];
	strcpy(tempBorrow, borrowDate);
	strcpy(tempReturn, returnDate);
	
	char *piece = strtok(tempBorrow, ".");  //dissecting the date into years, months and days
    if(piece != NULL){
        d1 = atoi(piece);
        piece= strtok(NULL, ".");
        if(piece != NULL){
        	m1=atoi(piece);
        	
        	piece= strtok(NULL, ".");
        	if(piece != NULL){
        		y1= atoi(piece);
			}
		}
	}
	
	piece = strtok(tempReturn, ".");
    if(piece != NULL){
        d2 = atoi(piece);
        piece= strtok(NULL, ".");
        if(piece != NULL){
        	m2=atoi(piece);
        	
        	piece= strtok(NULL, ".");
        	if(piece != NULL){
        		y2= atoi(piece);
			}
		}
	}
	
	total1= y1* 365+ d1;  //calculating the days of the years
	for(i=0; i<m1 -1; i++){  //calculating the days of the month
		total1 += monthDays[i];
	}
	
	if(m1 > 2 && (y1 % 4 ==0 && (y1 % 100 != 0 || y1 % 400 ==0))){  //checking for the leap years
		total1++;
	}
	
	total2= y2* 365+ d2;
	for(i=0; i<m2 -1; i++){
		total2 += monthDays[i];
	}
	
	if(m2 > 2 && (y2 % 4 ==0 && (y2 % 100 != 0 || y2 % 400 ==0))){
		total2++;
	}
	return total2-total1;
	
}

void saveBorrowsToCSV(Borrow *logs, int borrowCount){
	int i;
	FILE *file = fopen("BookBorrow.csv", "w");
	if(file==NULL){
		printf("Cannot open the file.\n");
		return;
	}
	
	for(i=0; i< borrowCount; i++){
		fprintf(file, "%s,%s,%d,%s\n", logs[i].tagNumber, logs[i].studentId, logs[i].transactionType, logs[i].date);
	}
	fclose(file);
}

Borrow* loadBorrowsFromCSV(int *borrowCount){
	FILE *file = fopen("BookBorrow.csv", "r");
	if(file==NULL){
		printf("Cannot open the file.\n");
		return NULL;
	}
	char line[150];
	int lines=0, i=0;
	while(fgets(line, sizeof(line), file) != NULL) lines++;
	
	*borrowCount = lines;
	fclose(file);
	
	Borrow *logs= NULL;
	if(lines > 0){
		logs= (Borrow*)malloc(lines * sizeof(Borrow));
		file= fopen("BookBorrow.csv", "r");
		if(file != NULL){
			while(fgets(line, sizeof(line), file) != NULL && i< lines){
				char *piece = strtok(line, ",");
				if (piece != NULL) strcpy(logs[i].tagNumber, piece);
				
				piece = strtok(NULL, ",");
                if (piece != NULL) strcpy(logs[i].studentId, piece);
                
                piece = strtok(NULL, ",");
                if (piece != NULL) logs[i].transactionType = atoi(piece);
                
                piece = strtok(NULL, ",\n");
                if (piece != NULL) strcpy(logs[i].date, piece);
                i++;
			}
			fclose(file);
		}
	}
	return logs;
}

Borrow* addTransaction(Borrow *logs, int *borrowCount, char *tag, char *studentId, int type, char *date){
	int index;
	(*borrowCount)++;
	if(*borrowCount==1) logs= (Borrow*)malloc(sizeof(Borrow));
	else logs= (Borrow*)realloc(logs, (*borrowCount) * sizeof(Borrow));
	
	index= (*borrowCount) -1;
	strcpy(logs[index].tagNumber, tag);
	strcpy(logs[index].studentId, studentId);
	logs[index].transactionType =type;
	strcpy(logs[index].date, date);
	
	saveBorrowsToCSV(logs, *borrowCount);
	return logs;
}

Borrow* borrowBook(Book *bookHead, Student *student, int studentCount, Borrow *logs, int *borrowCount, char *targetTag, char *targetId, char *currentDate){
	Book *temp= bookHead;
	int isFound=0, i, totalCopies, borrowedCopies, index, j=0;
	
	while(temp!= NULL && isFound==0){
		i=0;
		totalCopies = temp->copyNumber;
		borrowedCopies=0;
		
		for(j=0; j< totalCopies; j++){
			if(temp->p[j].status ==BORROWED) borrowedCopies++;
		}
		
		while(i< temp->copyNumber && isFound==0){
			if(strcmp(temp->p[i].tagNumber, targetTag)==0){
				isFound=1;
				
				if(borrowedCopies== totalCopies) printf("Fail, all copies are borrowed.\n");
				else if(temp->p[i].status ==BORROWED) printf("This copy is already borrowed.\n");
				else{
					index=-1, j=0;
					while(index==-1 && j< studentCount){
						if(strcmp(student[j].studentId, targetId)==0) index=j;
						j++;
					}
					
					if(index==-1) printf("Student not found.\n");
					else if(student[index].libraryScore<0) printf("Book cannot be borrowed. Student score too low: %d\n", student[index].libraryScore);
					else{
						temp->p[i].status= BORROWED;
						strcpy(temp->p[i].data.studentId, targetId);
						
						logs= addTransaction(logs, borrowCount, targetTag, targetId, 0, currentDate);
						saveBookToCSV(bookHead);
					}
				}
			}
			i++;
		}
		if(isFound==0) temp=temp->next;
	}
	if(isFound==0) printf("Book not found.\n");
	return logs;
}

Borrow* returnBook(Book *bookHead, Student *student, int studentCount, Borrow *logs, int *borrowCount, char *targetTag, char *currentDate){
	Book *temp= bookHead;
	int isFound=0, i;
	
	while(temp != NULL && isFound==0){
		i=0;
		while(i< temp->copyNumber && isFound==0){
			if(strcmp(temp->p[i].tagNumber, targetTag)==0){
				isFound=1;
				
				if(temp->p[i].status ==ON_SHELF) printf("This book is already on the sheld.\n");
				else{
					char borrowedId[9];
					strcpy(borrowedId, temp->p[i].data.studentId);
					
					char borrowDate[11]= "";
					int logIndex= (*borrowCount)-1;
					int dateFound=0;
					
					while(logIndex >=0 && dateFound==0){
						if(strcmp(logs[logIndex].tagNumber, targetTag)==0 && logs[logIndex].transactionType==0){
							strcpy(borrowDate, logs[logIndex].date);
							dateFound=1;
						}
						logIndex--;
					}
					if(dateFound==1){
						int difference = calculateDateDifference(borrowDate, currentDate);
						if(difference > 15){
							int index=-1, k=0;
							while(index==-1 && k< studentCount){
								if(strcmp(student[k].studentId, borrowedId)==0) index=k;
								k++;
							}
							
							if(index!= -1){
								student[index].libraryScore -= 10;
								printf("Book returned %d days late. New student score: %d\n", difference-15 , student[index].libraryScore);
								saveStuToCSV(student, studentCount);
							}
						}
					}
					temp->p[i].status= ON_SHELF;
					strcpy(temp->p[i].data.statusOfBook, "ON_SHELF");
					logs= addTransaction(logs, borrowCount, targetTag, borrowedId, 1, currentDate);
					saveBookToCSV(bookHead);
				}
			}
			i++;
		}
		if(isFound==0) temp=temp->next;
	}
	if(isFound==0) printf("Book not found.\n");
	return logs;
}

//FREE

void freeAll(Student *student, Author *author, Book *book, BooksAuthors *matchArray, Borrow* borrowLogs){
	if(student != NULL) free(student);
	if(matchArray != NULL) free(matchArray);
	if(borrowLogs != NULL) free(borrowLogs);
	
	Author *tempAuthor;
	while(author != NULL){
		tempAuthor =author;
		author = author->next;
		free(tempAuthor);
	}
	
	Book *tempBook;
	while(book != NULL){
		tempBook= book;
		book =book->next;
		if(tempBook->p!= NULL){
			free(tempBook->p);
		}
		free(tempBook);
	}
}
