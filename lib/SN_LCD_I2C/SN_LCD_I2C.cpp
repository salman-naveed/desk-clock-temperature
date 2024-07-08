#include <SN_LCD_I2C.h>
#include <SN_Time.h>
#include <SN_DHT.h>
#include <SN_Logger.h>

#define LCD_ADDRESS 0x27
#define LCD_COLUMNS 16
#define LCD_ROWS 2
#define LCD_SDA 0
#define LCD_SCL 2

LiquidCrystal_I2C lcd(LCD_ADDRESS, LCD_COLUMNS, LCD_ROWS);  // Configure LiquidCrystal_I2C library with 0x27 address, 16 columns and 2 rows

String date;
String t;

const char * days[] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"} ;
const char * months[] = {"Jan", "Feb", "Mar", "Apr", "May", "June", "July", "Aug", "Sep", "Oct", "Nov", "Dec"} ;
const char * ampm[] = {"AM", "PM"} ;

char temperature[] = " 00.0 C"; // "Temp = 00.0 C  "
char humidity[]    = " 00 %"; // "RH   = 00.0 %  ";


void SN_LCD_I2C_Init() {
    lcd.begin(LCD_SDA, LCD_SCL);                    // Initialize I2C LCD module (SDA = GPIO0, SCL = GPIO2)
    lcd.backlight();                    // Turn backlight ON
    lcd.setCursor(0, 0);                // Set cursor to first column of first row
    lcd.print("Initializing...");       // Print message on LCD
    SN_Logger_Log(true, "SN_LCD_I2C", "LCD Initialized");
    delay(2000);                        // Wait for 2 seconds
    SN_LCD_I2C_Clear();                  // Clear LCD screen
}

void SN_LCD_I2C_Clear() {
    lcd.clear();                       // Clear LCD screen
    SN_Logger_Log(true, "SN_LCD_I2C", "LCD Cleared");
}

void SN_LCD_I2C_print(String message) {
    lcd.setCursor(0, 0);                // Set cursor to first column of first row
    lcd.print(message);                 // Print message on LCD
    SN_Logger_Log(true, "SN_LCD_I2C", "Printing message on LCD");
}

void SN_LCD_I2C_clockDisplay() {
    SN_Logger_Log(true, "SN_LCD_I2C", "Displaying clock on LCD");
    // ----------------- Get current date and time -----------------
    int y, mo, d, h, m, s;

    time_t tNow;
    
    tNow = now();
    
    y = year(tNow);
    mo = month(tNow);
    d = day(tNow);
    
    h = hour(tNow);
    m = minute(tNow);
    s = second(tNow);

    // ----------------- Get current temperature and humidity -----------------
    int Temp = SN_DHT_getTemperature() * 10;
    int RH = SN_DHT_getHumidity() * 10;

    if (isnan(RH) || isnan(Temp)) {
        // SN_LCD_I2C_Clear();

        lcd.setCursor(0, 2);
        lcd.print("DHT Sensor Error");
        
        return;
    }

    // if (Temp < 0) {
    //     temperature[6] = '-';
    //     Temp = abs(Temp);
    // }
    // else {
    //     temperature[6] = ' ';
    //     temperature[7] = (Temp / 100) % 10  + 48;
    //     temperature[8] = (Temp / 10)  % 10  + 48;
    //     temperature[10] = Temp % 10 + 48;
    //     temperature[11] = 223;        // Degree symbol ( °)
    // }
    // if (RH >= 1000) {
    //     humidity[6] = '1';
    // }
    // else {
    //     humidity[6] = ' ';
    //     humidity[7] = (RH / 100) % 10; //+ 48;
    //     humidity[8] = (RH / 10) % 10; //+ 48;
    //     humidity[10] = RH % 10;// + 48;
    // }

    if (Temp < 0) {
        temperature[0] = '-';
        Temp = abs(Temp);
    } else {
        temperature[0] = ' ';
    }

    temperature[1] = (Temp / 100) % 10 + '0';
    temperature[2] = (Temp / 10) % 10 + '0';
    temperature[4] = Temp % 10 + '0';

    humidity[1] = (RH / 100) % 10 + '0';
    humidity[2] = (RH / 10) % 10 + '0';
    // humidity[4] = RH % 10 + '0';

    // ----------------- Display date and time on LCD -----------------

    SN_LCD_I2C_Clear();

    lcd.setCursor(0, 0);

    // // lcd.print("Date: ");
    // lcd.print(d);
    // lcd.print("/");
    // lcd.print(mo);
    // lcd.print("/");
    // lcd.print(y);

    

    // lcd.setCursor(0, 1);

    // // lcd.print("Time: ");
    // lcd.print(h);
    // lcd.print(":");
    // lcd.print(m);
    // lcd.print(":");
    // lcd.print(s);

    if (d < 10) lcd.print('0');
    lcd.print(d);
    lcd.print("/");
    if (mo < 10) lcd.print('0');
    lcd.print(mo);
    lcd.print("/");
    if (y < 10) lcd.print('0');
    lcd.print(y);
    lcd.print(" ");
    lcd.print(temperature);

    lcd.setCursor(0, 1);
    if (h < 10) lcd.print('0');
    lcd.print(h);
    lcd.print(":");
    if (m < 10) lcd.print('0');
    lcd.print(m);
    lcd.print(":");
    if (s < 10) lcd.print('0');
    lcd.print(s);
    lcd.print(" ");
    lcd.print(humidity);

    // ----------------- Display temperature and humidity on LCD -----------------

    // lcd.setCursor(0, 2);
    // lcd.print(temperature);

    // lcd.setCursor(0, 3);
    // lcd.print(humidity);



    
}