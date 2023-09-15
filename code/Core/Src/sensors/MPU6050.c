#include "sensors/MPU6050.h"

#include "i2c.h"

// https://controllerstech.com/how-to-interface-mpu6050-gy-521-with-stm32
// https://wiki.dfrobot.com/How_to_Use_a_Three-Axis_Accelerometer_for_Tilt_Sensing

const float M_PI = 3.14159265358979323846;

void MPU6050_Config(void) {
    
    uint8_t check, data;

    HAL_I2C_Mem_Read (&hi2c2, MPU6050_ADDR,WHO_AM_I_REG,1, &check, 1, 1000);

    if (check == 104) // WHO_AM_I should always be 0x68
	{
	    // power management register 0X6B we should write all 0's to wake the sensor up
	    data = 0;
	    HAL_I2C_Mem_Write(&hi2c2, MPU6050_ADDR, PWR_MGMT_1_REG, 1,&data, 1, 1000);

	    // set DATA RATE of 1KHz by writing SMPLRT_DIV register
	    data = 0x07;
	    HAL_I2C_Mem_Write(&hi2c2, MPU6050_ADDR, SMPLRT_DIV_REG, 1, &data, 1, 1000);

	    // set accelerometer configuration in ACCEL_CONFIG Register
	    // XA_ST=0,YA_ST=0,ZA_ST=0, FS_SEL=0 -> ± 2g
	    data = 0x00;
	    HAL_I2C_Mem_Write(&hi2c2, MPU6050_ADDR, ACCEL_CONFIG_REG, 1, &data, 1, 1000);

	    // set Gyroscopic configuration in GYRO_CONFIG Register
	    // XG_ST=0,YG_ST=0,ZG_ST=0, FS_SEL=0 -> ± 250 °/s
	    data = 0x00;
	    HAL_I2C_Mem_Write(&hi2c2, MPU6050_ADDR, GYRO_CONFIG_REG, 1, &data, 1, 1000);
	}
}

void MPU6050_Read_Accel (int16_t* Gx, int16_t* Gy, int16_t* Gz) {
	uint8_t Rec_Data[6];

	// Read 6 BYTES of data starting from GYRO_XOUT_H register
	HAL_I2C_Mem_Read (&hi2c2, MPU6050_ADDR, ACCEL_XOUT_H_REG, 1, Rec_Data, 6, 1000);

	// NOTE gyro es la velocidad angular, no tal cual el angulo
	// se ocupan otras funciones para obtener este valor

	int16_t Gyro_X_RAW = (int16_t)(Rec_Data[0] << 8 | Rec_Data [1]);
	int16_t Gyro_Y_RAW = (int16_t)(Rec_Data[2] << 8 | Rec_Data [3]);
	int16_t Gyro_Z_RAW = (int16_t)(Rec_Data[4] << 8 | Rec_Data [5]);

	*Gx = Gyro_X_RAW/131;
	*Gy = Gyro_Y_RAW/131;
	*Gz = Gyro_Z_RAW/131;
}

int8_t MPU6050_Roll(int16_t Gx, int16_t Gy, int16_t Gz) {
    return (atan2(Gy, Gz) * 180) / M_PI;
}


int8_t MPU6050_Pitch(int16_t Gx, int16_t Gy, int16_t Gz) {
    return (atan2(-Gx, sqrt(Gy * Gy + Gz * Gz)) * 180) / M_PI;
}
