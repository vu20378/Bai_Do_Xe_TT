/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdio.h"
#include "math.h"
#include "rc522.h"
#include "string.h"
#include "flash.h"

#define BLOCK_ADDR 0x04
#define MAX_VITRI 6
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
SPI_HandleTypeDef hspi1;

TIM_HandleTypeDef htim1;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI1_Init(void);
static void MX_TIM1_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

void Lcd_Cauhinh(void);
void Lcd_Ghi_Lenh(char lenh);
void Lcd_Ghi_Dulieu(char data);
void Lcd_Ghi_Lenh (char malenh);
void Lcd_Ghi_chuoi (char *str);
	
void Lcd_Ghi_Lenh (char malenh) 
{ 
 HAL_GPIO_WritePin(GPIOA,GPIO_PIN_11,GPIO_PIN_RESET); 
 HAL_GPIO_WritePin(GPIOA,GPIO_PIN_12,GPIO_PIN_RESET); 
	
 HAL_GPIO_WritePin(GPIOA,GPIO_PIN_10,GPIO_PIN_SET);
 HAL_GPIO_WritePin(GPIOB,GPIO_PIN_15, (malenh>>4)&0x01); 
 HAL_GPIO_WritePin(GPIOB,GPIO_PIN_14, (malenh>>5)&0x01); 
 HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12, (malenh>>6)&0x01); 
 HAL_GPIO_WritePin(GPIOB,GPIO_PIN_13, (malenh>>7)&0x01); 
 HAL_GPIO_WritePin(GPIOA,GPIO_PIN_10,GPIO_PIN_RESET);
 HAL_Delay(5);
	
 HAL_GPIO_WritePin(GPIOA,GPIO_PIN_10,GPIO_PIN_SET);
 HAL_GPIO_WritePin(GPIOB,GPIO_PIN_15, (malenh&0x01)); 
 HAL_GPIO_WritePin(GPIOB,GPIO_PIN_14, (malenh>>1)&0x01); 
 HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12, (malenh>>2)&0x01); 
 HAL_GPIO_WritePin(GPIOB,GPIO_PIN_13, (malenh>>3)&0x01); 
 HAL_GPIO_WritePin(GPIOA,GPIO_PIN_10,GPIO_PIN_RESET);
 HAL_Delay(5);
} 

void Lcd_Ghi_Dulieu (char dulieu) 
{ 
 HAL_GPIO_WritePin(GPIOA,GPIO_PIN_11,GPIO_PIN_RESET);
 HAL_GPIO_WritePin(GPIOA,GPIO_PIN_12,GPIO_PIN_SET);
	
 HAL_GPIO_WritePin(GPIOA,GPIO_PIN_10,GPIO_PIN_SET);
 HAL_GPIO_WritePin(GPIOB,GPIO_PIN_15, (dulieu>>4)&0x01); 
 HAL_GPIO_WritePin(GPIOB,GPIO_PIN_14, (dulieu>>5)&0x01); 
 HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12, (dulieu>>6)&0x01); 
 HAL_GPIO_WritePin(GPIOB,GPIO_PIN_13, (dulieu>>7)&0x01); 
 HAL_GPIO_WritePin(GPIOA,GPIO_PIN_10,GPIO_PIN_RESET);
 HAL_Delay(5);
	
 HAL_GPIO_WritePin(GPIOA,GPIO_PIN_10,GPIO_PIN_SET); 
 HAL_GPIO_WritePin(GPIOB,GPIO_PIN_15, (dulieu&0x01)); 
 HAL_GPIO_WritePin(GPIOB,GPIO_PIN_14, (dulieu>>1)&0x01); 
 HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12, (dulieu>>2)&0x01); 
 HAL_GPIO_WritePin(GPIOB,GPIO_PIN_13, (dulieu>>3)&0x01); 
 HAL_GPIO_WritePin(GPIOA,GPIO_PIN_10,GPIO_PIN_RESET); 
 HAL_Delay(5);
} 

void Lcd_Ghi_chuoi (char *str) 
{ 
 while(*str) 
 { 
 Lcd_Ghi_Dulieu(*str); 
 str++; 
 } 
}

void Lcd_Cauhinh (void) 
{ 
 Lcd_Ghi_Lenh(0x03);
 Lcd_Ghi_Lenh(0x02);
 Lcd_Ghi_Lenh(0x28);
 Lcd_Ghi_Lenh(0x06); 
 Lcd_Ghi_Lenh(0x0c);
 Lcd_Ghi_Lenh(0x01);
}	


int i = 0;
uint8_t strID[4];
uint8_t checkMasterID[4];
uint8_t checkSlaveID[4];
uint8_t IDmaster[4] = {0xD3, 0x5C, 0xC2, 0x36};
int countvitri = MAX_VITRI;

/*#pragma pack(1)
typedef struct{
		uint8_t UID[4];
		uint8_t checkMaster; 
		uint8_t checkInOut;
	} RFID;
#pragma pack()*/

RFID* cardID_Database = NULL;

uint8_t countofcardID = 0;
	
void addCardID(uint8_t uid[4], uint8_t checkMaster, uint8_t checkInOut){
	RFID newcard;
	memcpy(newcard.UID, uid, sizeof(newcard.UID));
	newcard.checkMaster = checkMaster;
	newcard.checkInOut = checkInOut;
	
	cardID_Database = realloc(cardID_Database, (countofcardID + 1)*sizeof(RFID));
	
	if(cardID_Database == NULL){
			Lcd_Ghi_Lenh(0x01);
			Lcd_Ghi_Lenh(0x80);
			Lcd_Ghi_chuoi("Bo nho day");
			HAL_Delay(2000);
			exit(EXIT_FAILURE);
		}
	cardID_Database[countofcardID] = newcard;
	countofcardID ++;
		if(checkMaster == 0){
				Lcd_Ghi_Lenh(0x01);
				Lcd_Ghi_Lenh(0x80);
				Lcd_Ghi_chuoi("Them Thanh Cong");
				HAL_Delay(2000);
			}
	}

int compareCardID(uint8_t uid[4]){
				for(i = 0; i < countofcardID; i++){
					if(memcmp(uid, cardID_Database[i].UID, sizeof(cardID_Database[i].UID)) == 0){
						return i;
					}
				}
				return -1;
	}
	
void freeMemory(){
	free(cardID_Database);
	}

void themID();
void xoaID();
void resetID();
	
int checkmaster(){
		if (!MFRC522_Request(PICC_REQIDL, checkMasterID)) {
			if (!MFRC522_Anticoll(checkMasterID)) {
				if(compareCardID(checkMasterID) == 0){
						return 1;
					}
				}
			}
		return 0;
	}
	
void menuthem(){
			Lcd_Ghi_Lenh(0x01);
			Lcd_Ghi_Lenh(0x80);
			Lcd_Ghi_chuoi("> 1. Them ID");
			Lcd_Ghi_Lenh(0xC0);
			Lcd_Ghi_chuoi("2. Xoa ID");
			HAL_Delay(1000);
	}

	void menuxoa(){
			Lcd_Ghi_Lenh(0x01);
			Lcd_Ghi_Lenh(0x80);
			Lcd_Ghi_chuoi("1. Them ID");
			Lcd_Ghi_Lenh(0xC0);
			Lcd_Ghi_chuoi("> 2. Xoa ID");
			HAL_Delay(1000);
	}
	
	void menureset(){
			Lcd_Ghi_Lenh(0x01);
			Lcd_Ghi_Lenh(0x80);
			Lcd_Ghi_chuoi("> 3. Reset ID");
			Lcd_Ghi_Lenh(0xC0);
			Lcd_Ghi_chuoi("4. Thoat");
			HAL_Delay(1000);
	}
	
	void menuthoat(){
			Lcd_Ghi_Lenh(0x01);
			Lcd_Ghi_Lenh(0x80);
			Lcd_Ghi_chuoi("3. Reset ID");
			Lcd_Ghi_Lenh(0xC0);
			Lcd_Ghi_chuoi("> 4. Thoat");
			HAL_Delay(1000);
	}
	
	void xoatempID(uint8_t *ID){
			ID = NULL;
		}
	
	
void menu(){
		xoatempID(strID);
		xoatempID(checkMasterID);
		Lcd_Ghi_Lenh(0x01);
		Lcd_Ghi_Lenh(0x80); 
		Lcd_Ghi_chuoi("MENU HE THONG");
		HAL_Delay(2000);

		int btnlen = 1;
		while(1){
			if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_14) == 0){
				while(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_14) == 0);
				btnlen++;	
			}
			
			if(btnlen == 1){
				menuthem();
				if(checkmaster()){
						xoatempID(checkMasterID);
						themID();
						btnlen = 1;
					}
				}
			else if(btnlen == 2){
				menuxoa();
				if(checkmaster()){
						xoatempID(checkMasterID);
						xoaID();
						btnlen = 1;
					}
				}
			else if(btnlen == 3){
				menureset();
				if(checkmaster()){
						xoatempID(checkMasterID);
						resetID();
						btnlen = 1;
					}
				}
			else if(btnlen == 4){
				menuthoat();
				if(checkmaster()){
						break;
					}
				}
			else if(btnlen > 4){
				btnlen = 1;
				}		
		}
	}

	void themID(){
		while(1){
				Lcd_Ghi_Lenh(0x01);
				Lcd_Ghi_Lenh(0x80); 
				Lcd_Ghi_chuoi("Moi nhap the");
				HAL_Delay(500);
				if (!MFRC522_Request(PICC_REQIDL, strID)) {
					if (!MFRC522_Anticoll(strID)) {
						if(compareCardID(strID) == 0){
								Lcd_Ghi_Lenh(0x01);
								Lcd_Ghi_Lenh(0x80); 
								Lcd_Ghi_chuoi("Ton tai the");
								HAL_Delay(1000);
							}
						else if(compareCardID(strID) == -1){
								addCardID(strID, 0, 0);
							}
						else{
								Lcd_Ghi_Lenh(0x01);
								Lcd_Ghi_Lenh(0x80); 
								Lcd_Ghi_chuoi("Ton tai the");
								HAL_Delay(1000);
							}
					}
				}
				
			if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_15) == 0){
					break;
				}
			}
	}
	
	void xoaID(){
		while(1){
			Lcd_Ghi_Lenh(0x01);
			Lcd_Ghi_Lenh(0x80); 
			Lcd_Ghi_chuoi("Moi nhap the");
			HAL_Delay(500);
			
			if (!MFRC522_Request(PICC_REQIDL, strID)) {
				if (!MFRC522_Anticoll(strID)) {
					int temp = compareCardID(strID);
					if( temp == 0){
							Lcd_Ghi_Lenh(0x01);
							Lcd_Ghi_Lenh(0x80); 
							Lcd_Ghi_chuoi("Khong the xoa");
							Lcd_Ghi_Lenh(0xC0); 
							Lcd_Ghi_chuoi("ID MASTER");
							HAL_Delay(1000);
						}
					else if(temp == -1){
								Lcd_Ghi_Lenh(0x01);
								Lcd_Ghi_Lenh(0x80); 
								Lcd_Ghi_chuoi("Khong ton tai");
								HAL_Delay(1000);;
						}
					else{
						if(cardID_Database[temp].checkInOut == 1){
								cardID_Database[temp].checkInOut = 0;
								countvitri++;
								Lcd_Ghi_Lenh(0x01);
								Lcd_Ghi_Lenh(0x80);
								Lcd_Ghi_chuoi("--  Moi Xe Ra --");
								__HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_2, 100);
								HAL_Delay(2000);
								__HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_2, 50);
							}
						for(i = temp; i < countofcardID; i++){
								cardID_Database[i] = cardID_Database[i+1];
							}
						
						cardID_Database = realloc(cardID_Database, (countofcardID - 1)*sizeof(RFID));
						if(cardID_Database == NULL){
									Lcd_Ghi_Lenh(0x01);
									Lcd_Ghi_Lenh(0x80);
									Lcd_Ghi_chuoi("Bo nho day");
									HAL_Delay(2000);
									exit(EXIT_FAILURE);
							}
						countofcardID--;
						Lcd_Ghi_Lenh(0x01);
						Lcd_Ghi_Lenh(0x80); 
						Lcd_Ghi_chuoi("Xoa Thanh Cong");
						HAL_Delay(2000);
					}
				}
			}
				
			if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_15) == 0){
					break;
				}
		}
	}
	
	void resetID(){
    if (countofcardID >= 2) {
        freeMemory();
				cardID_Database = NULL;
				countofcardID = 0;
				for(i = 0; i < sizeof(cardID_Database); i++){
						cardID_Database[i].checkInOut = 0;
					}
				
				countvitri = MAX_VITRI;
				Lcd_Ghi_Lenh(0x01);
				Lcd_Ghi_Lenh(0x80);
				Lcd_Ghi_chuoi("--  Moi Xe Ra --");
				__HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_2, 100);
				HAL_Delay(5000);
				__HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_2, 50);
			
				addCardID(IDmaster, 1, 0);

        Lcd_Ghi_Lenh(0x01);
        Lcd_Ghi_Lenh(0x80);
        Lcd_Ghi_chuoi("Xoa toan bo ID");
				Lcd_Ghi_Lenh(0xC0);
        Lcd_Ghi_chuoi("Thanh Cong");
        HAL_Delay(2000);
    } else {
        Lcd_Ghi_Lenh(0x01);
        Lcd_Ghi_Lenh(0x80);
        Lcd_Ghi_chuoi("Khong thanh cong");
        HAL_Delay(2000);
    }
	}
	
	char arrvitri[MAX_VITRI];
	char lcdvitri[32];
	uint16_t arrportpinx[] = {GPIO_PIN_3, GPIO_PIN_4, GPIO_PIN_5, GPIO_PIN_6, GPIO_PIN_7, GPIO_PIN_8};
	
	void manhinhvitri(){
		Lcd_Ghi_Lenh(0X80);
		Lcd_Ghi_chuoi("VTRI:1 2 3 4 5 6");
		sprintf(lcdvitri, " [%d] %c %c %c %c %c %c", countvitri, arrvitri[0], arrvitri[1], arrvitri[2], arrvitri[3], arrvitri[4], arrvitri[5]);
		Lcd_Ghi_Lenh(0xC0);
		Lcd_Ghi_chuoi(lcdvitri);
		}
	
	void docvitri(){
		for(i = 0; i < sizeof(arrportpinx); i++){
				if(HAL_GPIO_ReadPin(GPIOB, arrportpinx[i]) == 0){
					 arrvitri[i] = 'X';
					}
				else{
					arrvitri[i] = '0';
					}
				}
		manhinhvitri();
		}
	
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_SPI1_Init();
  MX_TIM1_Init();
  /* USER CODE BEGIN 2 */
	MFRC522_Init();
	Lcd_Cauhinh();
	
	for(int i = 0; i < 3; i++){
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, 1);
		HAL_Delay(500);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, 0);
		HAL_Delay(500);
	}
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, 1);
	HAL_Delay(2000);
	flash_erase(ADDRESS_DATA_STORAGE);
	//flash_read_Rfid(ADDRESS_DATA_STORAGE, cardID_Database);
	
	addCardID(IDmaster, 1, 0);
	
	Lcd_Ghi_Lenh(0x01);
	Lcd_Ghi_Lenh(0X80);
	Lcd_Ghi_chuoi("--- XIN CHAO ---");
	Lcd_Ghi_Lenh(0XC0);
	Lcd_Ghi_chuoi("---  MASTER  ---");	
	HAL_Delay(5000); 
	
	Lcd_Ghi_Lenh(0x01);
	Lcd_Ghi_Lenh(0X80);
	Lcd_Ghi_chuoi("--- XIN CHAO ---");
	Lcd_Ghi_Lenh(0XC0);
	Lcd_Ghi_chuoi("--- QUY KHACH --");	
	HAL_Delay(5000); 
	
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
	
	__HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_1, 50);
	__HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_2, 50);
	
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {		
		//flash_erase(ADDRESS_DATA_STORAGE);
		//flash_write_Rfid(ADDRESS_DATA_STORAGE, cardID_Database);
		if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1) == 1){
				HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, 1);
			}
		else{
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, 0);
			}
		
		if (!MFRC522_Request(PICC_REQIDL, strID)) { //true neu gui yeu cau quet thanh cong
			if (!MFRC522_Anticoll(strID)) {
					int temp = compareCardID(strID);
					if(temp == 0){
							menu();
						}
					else if(temp == -1){
							Lcd_Ghi_Lenh(0x01);
							Lcd_Ghi_Lenh(0x80);
							Lcd_Ghi_chuoi("- KHONG TON TAI-");
							HAL_Delay(2000);
						}
					else{
							if(cardID_Database[temp].checkInOut == 0 && countvitri > 0){
									countvitri--;
									if(countvitri <= 0) countvitri = 0;
									Lcd_Ghi_Lenh(0x01);
									Lcd_Ghi_Lenh(0x80);
									Lcd_Ghi_chuoi("-- Moi Xe Vao --");
									__HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_1, 100);
									HAL_Delay(5000);
									cardID_Database[temp].checkInOut = 1;
									__HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_1, 50);
									
								}
							else if(cardID_Database[temp].checkInOut == 1 && countvitri < MAX_VITRI){
									countvitri++;
									if(countvitri >= MAX_VITRI) countvitri = MAX_VITRI;
									Lcd_Ghi_Lenh(0x01);
									Lcd_Ghi_Lenh(0x80);
									Lcd_Ghi_chuoi("--  Moi Xe Ra --");
									flash_write_Rfid(ADDRESS_DATA_STORAGE, cardID_Database);
									__HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_2, 100);
									HAL_Delay(5000);
									cardID_Database[temp].checkInOut = 0;
									__HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_2, 50);
								}
						}
					}
				}
		
		if(countvitri == 0){
				Lcd_Ghi_Lenh(0x01);
				Lcd_Ghi_Lenh(0X80);
				Lcd_Ghi_chuoi("--- HE THONG ---");
				Lcd_Ghi_Lenh(0XC0);
				Lcd_Ghi_chuoi("---- HET CHO ---");	
				HAL_Delay(500);
			}
		else{
			docvitri();
			}
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
	freeMemory();
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief TIM1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM1_Init(void)
{

  /* USER CODE BEGIN TIM1_Init 0 */

  /* USER CODE END TIM1_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};
  TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = {0};

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 159;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 999;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
  sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
  if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
  sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
  sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
  sBreakDeadTimeConfig.DeadTime = 0;
  sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
  sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
  sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
  if (HAL_TIMEx_ConfigBreakDeadTime(&htim1, &sBreakDeadTimeConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */

  /* USER CODE END TIM1_Init 2 */
  HAL_TIM_MspPostInit(&htim1);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, LED_Ban_Dem_Pin|SDA_Pin|LCD_E_Pin|LCD_RW_Pin
                          |LCD_RS_Pin|LED_NAP_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, LCD_D6_Pin|LCD_D7_Pin|LCD_D5_Pin|LCD_D4_Pin
                          |Coi_Bao_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : BTN_EXIT_INT_Pin BTN_Menu_Pin BTN_Back_Pin */
  GPIO_InitStruct.Pin = BTN_EXIT_INT_Pin|BTN_Menu_Pin|BTN_Back_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : SS_Lua_Pin */
  GPIO_InitStruct.Pin = SS_Lua_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(SS_Lua_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : SS_BanDem_Pin */
  GPIO_InitStruct.Pin = SS_BanDem_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(SS_BanDem_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : LED_Ban_Dem_Pin SDA_Pin LCD_E_Pin LCD_RW_Pin
                           LCD_RS_Pin LED_NAP_Pin */
  GPIO_InitStruct.Pin = LED_Ban_Dem_Pin|SDA_Pin|LCD_E_Pin|LCD_RW_Pin
                          |LCD_RS_Pin|LED_NAP_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : LCD_D6_Pin LCD_D7_Pin LCD_D5_Pin LCD_D4_Pin
                           Coi_Bao_Pin */
  GPIO_InitStruct.Pin = LCD_D6_Pin|LCD_D7_Pin|LCD_D5_Pin|LCD_D4_Pin
                          |Coi_Bao_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : ViTri_1_Pin ViTri_2_Pin ViTri_3_Pin ViTri_4_Pin
                           ViTri_5_Pin ViTri_6_Pin */
  GPIO_InitStruct.Pin = ViTri_1_Pin|ViTri_2_Pin|ViTri_3_Pin|ViTri_4_Pin
                          |ViTri_5_Pin|ViTri_6_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI0_IRQn, 1, 0);
  HAL_NVIC_EnableIRQ(EXTI0_IRQn);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
