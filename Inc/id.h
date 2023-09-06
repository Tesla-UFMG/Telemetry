/*
 * NK523 can network identifiers - ID.h
 *
 *      Data: 16 de junho, 2023
 *      Autor: Leonardo de Sá
 *		Contato: (31) 98211-8390 || @oleonardodesa
 *
 *		link:
 *		https://docs.google.com/spreadsheets/d/1BHpB2Rvjlr6YgL0pbwQGXt2S_yGD1l4ZwxyoXBlnmko/edit?usp=sharing
 *
 *		ID          |   ORIGIN
 *		0 - 75      |   *NULL*
 *		76 - 150	|   Control
 *		151 - 225	|   Data Acquisition
 *		226 - 300	|   Safety System
 *		301 - 375	|   Packs BMS
 *		375 - 400	|   *NULL*
 */

#ifndef INC_ID_H_
#define INC_ID_H_

/* --- Control --- */
/* ECU */
#define ID_control_speed_average 76 //VELOCIDADE >< VOLANTE >< ACELERADOR >< FREIO
#define ID_control_hodometer 77 //
#define ID_control_torque_motor 78 //
#define ID_control_speed_wheel 79 //
#define ID_control_torque_reduction 80 //
#define ID_control_braking 81 //

/* Left Inversor */
#define ID_control_speed_l_motor 85 //
#define ID_control_energy_l_motor 86 //
#define ID_control_lost_msg_l_motor 87 //
#define ID_control_state_l_inv 88 //

/* Right Inversor */
#define ID_control_speed_r_motor 95 //
#define ID_control_energy_r_motor 96 //
#define ID_control_lost_msg_r_motor 97 //
#define ID_control_state_r_inv 98 //

#define ID_control_accelerometer 291 //
#define ID_control_gyroscopic 292 //
/* --------------- */

/* --- Data Acquisition --- */
/* Telemetry */
#define ID_acquisition_beacon 151 //

/* Coletora */
#define ID_acquisition_brake 152 //

/* Melexis */
#define ID_acquisition_mlx_de 153 //
#define ID_acquisition_mlx_dd 154 //
#define ID_acquisition_mlx_te 155 //
#define ID_acquisition_mlx_td 156 //
/* --------------- */

/* --- Safety System --- */
#define ID_safety_voltage 226 //
#define ID_safety_bms 227 //
#define ID_safety_current 230 //
#define ID_safety_charge 230 //
#define ID_safety_soc 232 //

/* Pack 1 */
#define ID_safety_pack1_1 301 //
#define ID_safety_pack1_2 302 //
#define ID_safety_pack1_3 303 //
#define ID_safety_pack1_4 304 //
#define ID_safety_pack1_5 305 //

/* Pack 2 */
#define ID_safety_pack2_1 306 //
#define ID_safety_pack2_2 307 //
#define ID_safety_pack2_3 308 //
#define ID_safety_pack2_4 309 //
#define ID_safety_pack2_5 310 //

/* Pack 3 */
#define ID_safety_pack3_1 311 //
#define ID_safety_pack3_2 312 //
#define ID_safety_pack3_3 313 //
#define ID_safety_pack3_4 314 //
#define ID_safety_pack3_5 315 //

/* Pack 4 */
#define ID_safety_pack4_1 316 //
#define ID_safety_pack4_2 317 //
#define ID_safety_pack4_3 318 //
#define ID_safety_pack4_4 319 //
#define ID_safety_pack4_5 320 //
/* --------------- */

#endif /* INC_ID_H_ */
