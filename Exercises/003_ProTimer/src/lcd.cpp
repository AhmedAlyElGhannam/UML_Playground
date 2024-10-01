#include "main.h"

// creating lcd object (RS [RW] EN D4 D5 D6 D7)
LiquidCrystal lcd(LCD_PIN_RS, LCD_PIN_RW, LCD_PIN_EN, LCD_PIN_D4, LCD_PIN_D5, LCD_PIN_D6, LCD_PIN_D7); 

void lcd_clear()
{
    lcd.clear();
}
void lcd_print_char(char c)
{
    lcd.print(c);
}
void lcd_scroll_left()
{
    lcd.scrollDisplayLeft();
}
void lcd_scroll_right()
{
    lcd.scrollDisplayRight();
}
void lcd_set_cursor(int r, int c)
{
    lcd.setCursor(c, r);
}
void lcd_no_auto_scroll()
{
    lcd.noAutoscroll();
}
void lcd_begin(uint8_t cols, uint8_t rows)
{
    lcd.begin(cols, rows);
}
void lcd_move_cursor_R_to_L()
{
    lcd.rightToLeft();
}
void lcd_move_cursor_L_to_R()
{
    lcd.leftToRight();
}
void lcd_cursor_off()
{
    lcd.noCursor();
}
void lcd_cursor_blinkoff()
{
    lcd.noBlink();
}
void lcd_print_number(int num)
{
    lcd.print(num);
}
void lcd_print_string(String s)
{
    lcd.print(s);
}