#include<stdio.h>
#include<stdlib.h>

struct linkedlist{
	char plate_number[10];
	int value; // mileage, or expected return date, or none
	struct linkedlist *next;
};

struct linkedlist *available_for_rent = NULL;
struct linkedlist *rented = NULL;
struct linkedlist *in_repair = NULL;

int is_equal(char a[], char b[]){
	for(int i = 0; i < 10; i++){
		if(a[i] == '\0' && b[i] == '\0') return 1;
		if(a[i]!=b[i]) return 0;
	}
	return 1;
}


int exists(struct linkedlist **head, char plate_number[]){
	struct linkedlist *temp = *head;
	while(temp!=NULL){
		if(is_equal(temp->plate_number, plate_number)) return 1;
		temp = temp->next;
	}
	return 0;
}

int insert(struct linkedlist **head, char plate_number[], int value){
	if(exists(head,plate_number)){
		printf("ERROR: %s cannot be duplicate => Already exists!\n", plate_number);
		return 0;
	}
	struct linkedlist *node = (struct linkedlist*) malloc(sizeof(struct linkedlist));
	for(int i = 0; i < 10; i++) node->plate_number[i] = plate_number[i];
	node->value = value;
	node->next = NULL;
	if(*head == NULL){
		*head = node;
		return 1;
	} 
	struct linkedlist *temp = *head;
	if(temp->value > value){
		node->next = temp;
		*head = node;
		return 1;
	}
	while(temp->next != NULL && (temp->next)->value <= value){
		temp = temp->next;
	}
	struct linkedlist *nextnextptr = temp->next;
	temp->next = node;
	node->next = nextnextptr;
	return 1;
}



struct linkedlist* delete_from_ll(struct linkedlist **head, char plate_number[]){
	if(!exists(head, plate_number)){
		printf("ERROR: %s cannot be found\n", plate_number);
		return NULL;
	}
	struct linkedlist *temp = *head;
	
	if(is_equal(temp->plate_number, plate_number)){
		*head = temp->next;
	}
	else{
		struct linkedlist *prev = temp;
		temp = temp->next;
		while(!is_equal(temp->plate_number, plate_number)){
			prev = temp;
			temp = temp->next;
		}
		prev->next = temp->next;
	}
	temp->next = NULL;
	return temp;
}

void display(struct linkedlist **head){
	struct linkedlist *temp = *head;
	printf("[head] -> \t");
	while(temp!=NULL){
		printf("[");
		printf("%s,", temp->plate_number);
		printf("%d", temp->value);
		printf("] -> ");
		temp = temp->next;
	}
	printf("NULL\n");
}


void add(char plate_number[], int value, int option){
	int inserted = 0;
	if(option != 3 && option != 5){
		inserted = insert(&available_for_rent, plate_number, value);
		if(inserted) printf("ACTION: CAR ADDED TO available_for_rent list\n");
		else printf("ACTION: CAR NOT ADDED TO available_for_rent list\n");
	}
	else if(option == 5){
		if(available_for_rent == NULL){
			printf("ERROR: No cars available now!");
			return;
		}
		plate_number = available_for_rent->plate_number;
		struct linkedlist *node = delete_from_ll(&available_for_rent, plate_number);
		//free(node);
		inserted = insert(&rented,plate_number,value);
		if(inserted) printf("ACTION: CAR ADDED TO rented list\n");
		else printf("ACTION: CAR NOT ADDED TO rented list\n");
	}
	else{
		inserted = insert(&in_repair,plate_number,value);
		if(inserted) printf("ACTION: CAR ADDED TO in_repair list\n");
		else printf("ACTION: CAR NOT ADDED TO in_repair list\n");
	}
	if((option == 2 || option == 3) && inserted){
		if(exists(&rented, plate_number)){
			struct linkedlist *node = delete_from_ll(&rented, plate_number);
			free(node);
		}
		float ans = 80.0;
		if(value > 200){
			value -= 200;
			ans += (value * 0.15);
		}
		printf("Charges for this Transaction: $ %.2f\n", ans);
	}
}

void print_total_bill(){
	float ans = 0.0;
	struct linkedlist *temp = rented;
	while(temp != NULL){
		int value = temp->value;
		float tval = 80.0;
		if(value > 200){
			value -= 200;
			tval += (value * 0.15);
		}
		temp = temp->next;
		ans += tval;
	}
	printf("\nTotal income for rented cars: $ %.2f\n", ans);
}

void transfer(char plate_number[]){
	struct linkedlist *node = delete_from_ll(&in_repair, plate_number);
	if(node == NULL){
		printf("ACTION: TRANSFER CAR NOT SUCCESSFUL\n");
		return;
	}
	insert(&available_for_rent,node->plate_number,node->value);
	free(node);
	printf("ACTION: TRANSFER CAR from in_repair TO available_for_rent list\n");
}

void show_all(){
	printf("AVAILABLE FOR RENT: ");
	display(&available_for_rent);

	printf("RENTED: ");
	display(&rented);

	printf("IN REPAIR: ");
	display(&in_repair);
}


void load_ll(){
	FILE *in_file = fopen("available_for_rent.list", "r");	
	if (in_file == NULL){   
		printf("Error! Could not open file available_for_rent.list\n"); 
    	return;
    }
    char line[100]; 
    while(fgets(line, 100, in_file) != NULL){ 
    	char plate_number[10], temp[90];
    	int value, comma = 0, j = 0;
    	for(int i = 0; i < 100; i++){
    		if(line[i] == ','){
    			plate_number[j] = '\0';
    			comma = 1;
    			j = -1;
    		}
    		else if(line[i] == '\n'){
    			temp[j] = '\0';
    			value = atoi(temp);
    		}
    		else{
    			if(comma){
    				temp[j] = line[i];
    			}
    			else{
    				plate_number[j] = line[i];
    			}
    		}
    		j += 1;
    	}
    	insert(&available_for_rent, plate_number, value); 
    }
    fclose(in_file);
    
    in_file = fopen("rented.list", "r");	
	if (in_file == NULL){   
		printf("Error! Could not open file available_for_rent.list\n"); 
    	return;
    }
    while(fgets(line, 100, in_file) != NULL){ 
    	char plate_number[10], temp[90];
    	int value, comma = 0, j = 0;
    	for(int i = 0; i < 100; i++){
    		if(line[i] == ','){
    			plate_number[j] = '\0';
    			comma = 1;
    			j = -1;
    		}
    		else if(line[i] == '\n'){
    			temp[j] = '\0';
    			value = atoi(temp);
    		}
    		else{
    			if(comma){
    				temp[j] = line[i];
    			}
    			else{
    				plate_number[j] = line[i];
    			}
    		}
    		j += 1;
    	}
    	insert(&rented, plate_number, value); 
    }
    fclose(in_file);
    
    in_file = fopen("in_repair.list", "r");	
	if (in_file == NULL){   
		printf("Error! Could not open file available_for_rent.list\n"); 
    	return;
    }
    while(fgets(line, 100, in_file) != NULL){ 
    	char plate_number[10], temp[90];
    	int value, comma = 0, j = 0;
    	for(int i = 0; i < 100; i++){
    		if(line[i] == ','){
    			plate_number[j] = '\0';
    			comma = 1;
    			j = -1;
    		}
    		else if(line[i] == '\n'){
    			temp[j] = '\0';
    			value = atoi(temp);
    		}
    		else{
    			if(comma){
    				temp[j] = line[i];
    			}
    			else{
    				plate_number[j] = line[i];
    			}
    		}
    		j += 1;
    	}
    	insert(&in_repair, plate_number, value); 
    } 
    fclose(in_file);
}

void save_ll(){
	FILE *out_file = fopen("available_for_rent.list", "w");	
	if (out_file == NULL){   
		printf("Error! Could not open file available_for_rent.list\n"); 
    	return;
    }
    struct linkedlist *temp = available_for_rent;
    while(temp!=NULL){
    	fprintf(out_file, "%s,%d\n", temp->plate_number, temp->value);
    	temp = temp->next;
    }
    fclose(out_file);

    out_file = fopen("rented.list", "w");	
	if (out_file == NULL){   
		printf("Error! Could not open file rented.list\n"); 
    	return;
    }
    temp = rented;
    while(temp!=NULL){
    	fprintf(out_file, "%s,%d\n", temp->plate_number, temp->value);
    	temp = temp->next;
    }
    fclose(out_file);

    out_file = fopen("in_repair.list", "w");	
	if (out_file == NULL){   
		printf("Error! Could not open file in_repair.list\n"); 
    	return;
    }
    temp = in_repair;
    while(temp!=NULL){
    	fprintf(out_file, "%s,%d\n", temp->plate_number, temp->value);
    	temp = temp->next;
    }
    fclose(out_file);
}


int main(){
	
	load_ll(); //loads all the linkedlist from files
	printf("LOADED STATE FILE!!\n");
	show_all();

	int option = 0;
	while(option != 7){
		char plate_number[10];
		int mileage, expected_return_date;

		printf("\n\nEnter 1 to 7 as per the below given options:\n");
		printf("\t1. add a new car to the available-for-rent list\n");
		printf("\t2. add a returned car to the available-for-rent list\n");
		printf("\t3. add a returned car to the repair list,\n");
		printf("\t4. transfer a car from the repair list to the available-for-rent list,\n");
		printf("\t5. rent the first available car,\n");
		printf("\t6. print all the lists,\n");
		printf("\t7. quit\n");
		scanf("%d",&option);

		switch(option){
			case 1:
			case 2:
			case 3:
				printf("Enter the plate number:\t");
				scanf("%s", plate_number);
				printf("Enter the mileage:\t");
				scanf("%d", &mileage);
				add(plate_number, mileage, option);
				break;
			case 4:
				printf("Enter the plate number:\t");
				scanf("%s",plate_number);
				transfer(plate_number);
				break;
			case 5:
				printf("Enter Expected Return Date (in yymmdd format):\t");
				scanf("%d",&expected_return_date);
				add("",expected_return_date,option);
				break;
			case 6:
				show_all();
				break;
			case 7:
				break;
			default:
				printf("Error: Invalid Choice!\n");
				break;
		}
	}
	print_total_bill();

	save_ll(); //saves all the linkedlist to files
	printf("SAVED STATE FILE!!\n");
	show_all();
	return 0;
}