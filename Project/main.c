#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define SCHEDULE_MAX_LENGTH 100

// 월별 일 수
int date[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

// 스택 데이터 개수
int count;

// 스케줄 저장 배열
int mScheduleYear[SCHEDULE_MAX_LENGTH] = {0};
int mScheduleMonth[SCHEDULE_MAX_LENGTH] = {0};
int mScheduleDay[SCHEDULE_MAX_LENGTH] = {0};
char mSchedule[SCHEDULE_MAX_LENGTH][20] = {'\0'};

// 스케줄 갯수
int mScheduleCount = 0;

// 스택 함수
void push(char*, char);
void pushStr(char*, char*);
char pop(char*);
int isEmpty(char*);

// 메뉴 함수
void scheduler();
void calculator();

// 계산기 함수
char getVariable(char*);
int getValue(char*);
int getLength(char*);
int isOperation(char*);
void transformation(char *str, char *variable, int *value);
void replaceInt(char *str, int index, int value);

// 스케줄관리 함수
int checkDate(int, int, int);
int checkDay(int, int, int);
int checkMonth(int);
int schedulerMenu();
int getLeapYear(int, int);
void printCalendar(int, int, int);
int isLeapYear(int);
void insertSchedule(int);
void deleteSchedule();
void sort();

// String 함수
char* removeEnterInFgetsString(char*);
void copyStr(char*, char*);
int isEqual(char*, char*);




int main(void) {
	int menuChoice;
	int subMenu;

	while(1){
		printf("\tMenu\n");
		printf("1) 공학용 계산기\n");
		printf("2) 스케줄 관리\n");
		printf("3) 프로그램 종료\n");

		scanf("%d", &menuChoice);
		switch(menuChoice) {
			case 1:
				// 계산기
				calculator();
				break;

			case 2:
				while (1) {
					// 스케줄러
					subMenu = schedulerMenu();
					switch(subMenu) {
						case 1:
							insertSchedule(mScheduleCount);
							break;

						case 2:
							deleteSchedule();
							break;

						case 3:
							scheduler();
							break;

						case 4:
							system("clear");
							break;
					}
					if(subMenu == 4)
						break;
				}
				break;

			case 3:
				// 종료
				printf("종료됩니다.\n");
				break;
		}
		if(menuChoice == 3)
			break;
	}
	return 0;
}

// 스케쥴 관리 메뉴 출력
int schedulerMenu() {
	int menu;
	printf("1. 스케줄 입력\n");
	printf("2. 스케줄 삭제\n");
	printf("3. 스케줄 보기\n");
	printf("4. Main menu\n");
	printf("메뉴 : ");
	scanf("%d", &menu);
	return menu;
}


// fgets는 마지막에 \n도 저장되기에 \n을 제거.
char* removeEnterInFgetsString(char *str) {
	int length = getLength(str);
	str[length - 1] = '\0';
	return str;
}


// 문자열 같은지 비교
int isEqual(char* str1, char* str2) {
	while (*str1 != '\0' && *str2 != '\0') {
		if (*str1++ != *str2++)
			return 0;
	}
	if (*str1 != *str2)
		return 0;
	return 1;
}


// 문자열 복사
void copyStr(char* fromStr, char* toStr) {
	while(*fromStr != '\0')
		*toStr++ = *fromStr++;
	*toStr = 0;
}

// 스케줄 삭제
void deleteSchedule() {
	int year, month, day;
	char schedule[SCHEDULE_MAX_LENGTH];

	printf("입력 : ");
	scanf("%d %d %d", &year, &month, &day);
	getchar();
	fgets(schedule, sizeof(schedule), stdin);
	removeEnterInFgetsString(schedule);
	printf("출력 : %d년 %d월 %d일 %s\n",year,month,day,schedule);

	int i;
	// 년 월 일 스케줄에 매칭되는 스케줄 index 찾기
	for (i = 0; i < mScheduleCount; i++) {
		if (mScheduleYear[i] == year && mScheduleMonth[i] == month && mScheduleDay[i] == day && isEqual(mSchedule[i], schedule))
			break;
	}

	if (mScheduleYear[i] == year && mScheduleMonth[i] == month && mScheduleDay[i] == day && isEqual(mSchedule[i],schedule)) {
		for (int j = i + 1; j < mScheduleCount; j++) {
			mScheduleYear[j - 1] = mScheduleYear[j];
			mScheduleMonth[j - 1] = mScheduleMonth[j];
			mScheduleDay[j - 1] = mScheduleDay[j];
			copyStr(mSchedule[j], mSchedule[j -1]);
		}
		printf("%d년 %d월 %d일의 일정이 있습니다.\n",year,month,day);
		printf("일정을 삭제하였습니다.\n");
		mScheduleCount--;
		mScheduleYear[mScheduleCount] = 0;
	} else
	{
		printf("%d년 %d월 %d일의 일정이 없습니다.\n",year,month,day);
		printf("일치하는 일정이 없습니다.\n");
	}
	printf("아무키나 입력하세요.........");
	getchar();
}


// 스케줄 입력
void insertSchedule(int scheduleCount) {
	int i;
	int year, month, day;
	char schedule[100];
	while (1) {
		printf("입력 : ");
		scanf("%d %d %d", &year, &month, &day);
		getchar();
		fgets(schedule, sizeof(schedule), stdin);
		removeEnterInFgetsString(schedule);

		// 날짜 제대로 입력했는지 확인
		if (checkDate(year, month, day)) {
			break;
		}
	}
	printf("출력 : %d년 %d월 %d일 %s\n", year, month, day, schedule);

	// 일정 겹치는지 확인
	int overwrite = 0;
	for (i = 0; i < scheduleCount; i++) {
		// 겹침
		if (mScheduleYear[i] == year && mScheduleMonth[i] == month && mScheduleDay[i] == day) {
			overwrite = 1 ;
			char answer;
			printf("해당 날짜에 이미 일정이 있습니다. 일정을 추가하려면Yes, 덮어쓰려면 No를 입력하세요(Y or N)");
			answer = getchar();
			getchar();
			// 추가
			if (answer == 'Y' || answer == 'y') {
				break ;
			}
			// x되돌아가기 덮어쓰기
			else
			{
				overwrite = 1;
				scheduleCount = i;
				break;
			}
			//overwrite = 0 ;
			//break;
		}
	}

	// 덮어쓰지 않겠다면 처음으로 돌아감
	//if (!overwrite)
	//	scheduleCount++;	

	mScheduleYear[scheduleCount] = year;
	mScheduleMonth[scheduleCount] = month;
	mScheduleDay[scheduleCount] = day;
	copyStr(schedule, mSchedule[scheduleCount]);
	if (overwrite == 0) { // Yes를 입력했을때or 일정이 없어서 추가
		scheduleCount++;
		mScheduleCount++;
		printf("%d %d\n",scheduleCount,mScheduleCount);
		printf("일정을 추가하였습니다.\n");
		printf("아무키나 입력하세요.......");
	}
	
//	if (!overwrite)
//		mScheduleCount++;
	//if (mScheduleCount == scheduleCount)
	//	mScheduleCount++;
	else { // No를 입력했을때
		printf("일정을 덮어썼습니다.\n");
		printf("아무키나 입력하세요.......");
	}
	getchar();
	system("clear");
	//sort();
}

int checkDate(int year, int month, int day) {
	// 1 ~ 12월 입력했는지 확인
	if (!checkMonth(month)) {
		printf("1 ~ 12월로 입력해주세요.\n");
		return 0;
	} else if (!checkDay(year, month, day)) {
		printf("%d년 %d월 %d일은 존재하지 않습니다.\n", year, month, day);
		return 0;
	}
	return 1;
}


// 월 제대로 입력했는지 확인
int checkMonth(int month) {
	if (month >= 1 && month <= 12)
		return 1;
	return 0;
}


// 일 제대로 입력했는지 확인
int checkDay(int year, int month, int day) {
	int leap = isLeapYear(year);
	if (day > 0 && day <= date[month - 1])
		return 1;
	else if (month == 2 && leap)
		if (day > 0 && day <= 29)
			return 1;
	return 0;
}

// 일정 정렬하기
void sort() {
	int i, j, temp;
	char swap[20];
	for(i=0; i < mScheduleCount; i++) {
		for(j=0; j < mScheduleCount - i - 1; j++) {
			if(mScheduleYear[j] > mScheduleYear[j+1])
			{
				temp = mScheduleYear[j];
				mScheduleYear[j] = mScheduleYear[j+1];
				mScheduleYear[j+1] = temp;

				temp = mScheduleMonth[j];
				mScheduleMonth[j] = mScheduleMonth[j+1];
				mScheduleMonth[j+1] = temp;

				temp = mScheduleDay[j];
				mScheduleDay[j] = mScheduleDay[j+1];
				mScheduleDay[j+1] = temp;

				copyStr(mSchedule[j], swap);
				copyStr(mSchedule[j+1], mSchedule[j]);
				copyStr(swap, mSchedule[j+1]);


			}

			else if(mScheduleYear[j] == mScheduleYear[j+1])
			{

				if(mScheduleMonth[j] > mScheduleMonth[j+1])
				{
					temp = mScheduleYear[j];
					mScheduleYear[j] = mScheduleYear[j+1];
					mScheduleYear[j+1] = temp;

					temp = mScheduleMonth[j];
					mScheduleMonth[j] = mScheduleMonth[j+1];
					mScheduleMonth[j+1] = temp;

					temp = mScheduleDay[j];
					mScheduleDay[j] = mScheduleDay[j+1];
					mScheduleDay[j+1] = temp;

					copyStr(mSchedule[j], swap);
					copyStr(mSchedule[j+1], mSchedule[j]);
					copyStr(swap, mSchedule[j+1]);

				}		
			}
		}
	}
}




// 스케줄 보기
void scheduler() {

	//printf("%d",mScheduleCount);
	int i, year, day, month;
	getchar();
	printf("%d\n",mScheduleCount);
	for(i=0; i < mScheduleCount; i++)
	{

		if((mScheduleYear[i] == mScheduleYear[i+1]) && (mScheduleMonth[i] == mScheduleMonth[i+1]))
			continue;
		year = mScheduleYear[i];
		month = mScheduleMonth[i];
		day = mScheduleDay[i];

		printf("\t\t\t\t\t\t %d년 %d월\n\n", year, month);

		// 캘린더 출력
		printCalendar(year, month, day);
		while(1){
			char c;
			c = getchar();
			if(c == '\n')
				break;
		}
	}

}


/*
 * 캘린더 출력
 * @Params : year = 년도
 *			leapYear = 윤년 횟수
 * 			month = 월
 *			day = 일
 */
void printCalendar(int year, int month, int day) {
	// 1년1월1일로부터 해당 년월일까지 총 일 수 계산
	int i, j, k;
	int leapYear = getLeapYear(year, month);
	int allDay = year * 365 + leapYear;
	for (i = 0; i < month - 1; i++)
		allDay += date[i];

	// 캘린더 출력 시 시작 요일 계산
	int skipDay = allDay % 7;

	// 2월이면 윤년인지 확인해서 윤년이면 29일까지 출력
	int isLeap = 0;
	if (month == 2 && isLeapYear(year))
		isLeap = 1;

	// 캘린더 출력 (윤년의 2월이면 29일까지, 시작 요일 포함)
	int weekCount = 1;
	printf("Sun.\t\tMon.\t\tTues.\t\tWed\t\tThurs.\t\tFri.\t\tSat.\n");
	for (i = 1; i <= date[month - 1] + isLeap + skipDay; i++, weekCount++) {
		// 시작 요일 맞추기 위해 건너뜀
		if (i <= skipDay)
			printf("\t\t");

		// 날짜 출력
		else
			printf("%d\t\t", i - skipDay);

		// 토요일까지 모두 출력 후 또는 day 모두 출력 후 개행
		if (i % 7 == 0 || i == date[month - 1] + isLeap + skipDay) {
			printf("\n");
			// 날짜 밑에 일정 출력
			for (k = i - weekCount + 1; k <= i; k++) {
				weekCount = 0;
				// 해당 날짜에 일정 있는지 검사
				for (j = 0; j < mScheduleCount; j++) {
					// 일정 있으면 출력
					if (mScheduleYear[j] == year && mScheduleMonth[j] == month && mScheduleDay[j] == k - skipDay) {
						int len = getLength(mSchedule[j]);
						char str[100];
						copyStr(mSchedule[j],str);
						if(len >= 12) {
							str[12] = 0;
							printf("%s...", str);
						}
						else {
							printf("%s", mSchedule[j]);
						}
						break;
					}
				}
				printf("\t\t");
			}
			printf("\n");
		}
	}
	printf("\n\n");
}


// 윤년인지 확인
int isLeapYear(int year) {
	// 100으로 나누어 떨어지면 윤년아님
	if (year % 100 == 0) {
		// 그중에서 400으로 나누어 떨어지면 윤년임
		if (year % 400 == 0)
			return 1;
	} else if (year % 4 == 0)
		return 1;
	return 0;
}


// 윤년 몇 번 지나왔는지 계산
int getLeapYear(int year, int month) {
	int i;
	int leapYear = 0;
	int includeYear = 0;
	// 2월 지났으면 해당 년도도 포함해서 계산
	if (month > 2)
		includeYear = 1;
	for (i = 1; i < year + includeYear; i++) {
		if (isLeapYear(i))
			leapYear++;
	}
	return leapYear;
}


// 계산기
void calculator() {
	// 변수 지정 or 수학 식 임시 저장
	char str[100];

	// 변수 명 저장
	char variable[10];

	// 변수 값 저장
	int value[10];

	int variableCount = 0;

	// 연산자 임시 저장
	char operator;

	getchar();

	// 변수 지정
	while(1) {
		printf("Input : ");
		fgets(str, sizeof(str), stdin);
		removeEnterInFgetsString(str);
		if (getLength(str) == 0)
			break;

		// 변수식이면
		if (!isOperation(str)) {
			variable[variableCount] = getVariable(str);
			value[variableCount] = getValue(str);
			//            printf("variable = %c\n", variable[variableCount]);
			//            printf("value = %d\n", value[variableCount]);
			variableCount++;
		}

		// 연산식이면
		else {
			transformation(str, variable, value);
			break;
		}
	}
}


/*
 * 변수들 숫자로 변환
 * @params : *str = 연산식
 *           *variable = 변수 배열
 *           *value = 변수 값 배열
 */
void transformation(char *str, char *variable, int *value) {
	int index = 0;
	while (*str != '\0') {
		if (*str >= 'A' && *str <= 'Z') {
			replaceInt(str, index, value[index]);
			printf("Transformation : %s\n", str);
		}
		str++;
		index++;
	}
}


void replaceInt(char *str, int index, int value) {
	char valueStack[100];
	char valueStr[100];
	int a = 1;
	while (value / a) {
		int num = value % (a * 10);
		push(valueStack, num / a);
		printf("num = %d\n", num / a);

		a *= 10;
	}

	while (!isEmpty(valueStack)) {
		int index = 0;
		char c =  pop(valueStack);
		printf("%c\n", c);
		valueStr[index++] = pop(valueStack);
	}
	printf("valueStr = %s\n", valueStr);
}


//char convertIntTochar(int ) {

//}



/* 계산식인지 변수식인지 구분
 * @Param : *str = 식
 * @Return Value : 0 -> 변수식
 *				   1 -> 계산식 
 */
int isOperation(char *str) {
	while (*str++ != '\0')
		if (*str == '=')
			return 0;
	return 1;
}


// 중위표기법 -> 후위 표기법
void convertToLast() {

}


// 문자열 길이 얻는 함수
int getLength(char *str) {
	int count = 0;
	while(*str++ != '\0')
		count++;
	return count;
}


/*
 * 문자열 중 변수 추출
 * Input : A = 10
 * Output : A
 */
char getVariable(char *str) {
	while(*str == ' ')
		str++;
	return *str;
}


/*
 * 문자열 중 변수 값 추출
 * Input : A = 10
 * Output : 10
 */
int getValue(char *str) {
	char stack[100];

	int result = 0;
	int count = 1;
	while(*str < '0' || *str > '9')
		str++;
	while(*str >= '0' && *str <= '9') {
		push(stack, *str++);
	}
	while(!isEmpty(stack)) {
		char c = pop(stack);
		//        printf("%c\n", c);
		switch(c) {
			case '1':
				result += 1 * count;
				break;

			case '2':
				result += 2 * count;
				break;

			case '3':
				result += 3 * count;
				break;

			case '4':
				result += 4 * count;
				break;

			case '5':
				result += 5 * count;
				break;

			case '6':
				result += 6 * count;
				break;

			case '7':
				result += 7 * count;
				break;

			case '8':
				result += 8 * count;
				break;

			case '9':
				result += 9 * count;
				break;

		}
		count *= 10;
	}
	return result;
}


// 스택에 문자 하나 추가
void push(char* stack, char chr) {
	stack[count++] = chr;
}


// 스택에 문자열 추가
void pushStr(char* stack, char *str) {
	while(*str != '\0') {
		stack[count++] = *str++;
	}
}


// 스택에 있는 문자 하나 출력
char pop(char* stack) {
	return stack[--count];
}


// 스택이 비었는지 확인
int isEmpty(char *stack) {
	if (count == 0)
		return 1;
	return 0;
}
