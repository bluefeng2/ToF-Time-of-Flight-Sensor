#define MIN_DIST 30
#define MAX_DIST 1000 

void main(void) {
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_I2C1_Init();
    MX_TIM3_Init(); 

    VL53L0X_Dev_t sensor_dev;
    VL53L0X_init(&sensor_dev, &hi2c1); 
    VL53L0X_start_continuous(&sensor_dev);

    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_4);

    while (1) {
        uint16_t distance_mm;
        
        if (VL53L0X_read_distance(&sensor_dev, &distance_mm) == HAL_OK) {
            
            if (distance_mm < MIN_DIST) distance_mm = MIN_DIST;
            if (distance_mm > MAX_DIST) distance_mm = MAX_DIST;

            uint32_t pulse_value = (uint32_t)((distance_mm - MIN_DIST) * htim3.Init.Period / (MAX_DIST - MIN_DIST));
            
            __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_4, pulse_value);
        }

        HAL_Delay(50);
    }
}