#include "drivers.h"
#include "cfg_decoder.h"
#include "../crc/MF_CRC8.h"
#include "../crc/MF_CRC16.h"

void cfg_decoder_init( cfg_decoder_object_t *obj, uint8_t *p, uint16_t max )
{
    obj->step = 0;
    obj->p_data = p;
    obj->max = max;
}

bool cfg_decoder_upack( cfg_decoder_object_t *obj, uint8_t data )
{
    switch( obj->step ) {
        case 0:
			if( data == 0x55 ) {
				obj->idx = 0;
				obj->p_data[obj->idx++] = data;
                obj->step = 1;
			}
			break;
		case 1:
			obj->len = data;
			obj->p_data[obj->idx++] = data;
			obj->step = 2;
			break;
		case 2:
			obj->len |= ( ( data & 0x3 ) << 8 );
            obj->ver = ( data >> 2 );
			if( obj->len > obj->max ) {
				obj->step = 0;
			} else {
				obj->p_data[obj->idx++] = data;
				obj->step = 3;
			}
			break;
		case 3:
			obj->p_data[obj->idx++] = data;
			if( Verify_CRC8_Check_Sum( obj->p_data, 4 ) ) {
				obj->step = 4;
			} else {
				obj->step = 0;
			}
			break;
		case 4:
            if( obj->idx < obj->len ) {
                obj->p_data[obj->idx++] = data;
            }
            if( obj->idx == obj->len ) {
                obj->step = 0;
                if( Verify_CRC16_Check_Sum( obj->p_data, obj->len ) ) {
//                    if( obj->ver == 1 ) {
                        return true ;
//                    }
                }
            }
            break;
        default:
            obj->step = 0;
    }
    return false;
}

uint8_t control_flag = 0;
uint8_t get_control_flag(void)
{
  return control_flag;
}

void  Flow_Sensor_Unpack( cfg_decoder_object_t *obj, uint8_t data )
{
    switch( obj->step ) 
		{
      case 0:
			 if( data == 0x55 ) 
          obj->step = 1;
			break;
			 
		  case 1:
				 if( data == 0xaa ) 
          obj->step = 2;
				 else
					 obj->step = 0;
			break;
				 
		  case 2:
				 if( data == 0x66) 
          obj->step = 3;
				 else
					obj->step = 0;
			break;
				 
		 case 3:
				 if( data == 0xbb) 
          obj->step = 4;
				 else
					obj->step = 0;
		 break;
				 
		 case 4:
			 control_flag  = data;
       obj->step = 0;
     break;
        default:
            obj->step = 0;
    }
 
}









#if 0 
bool radar_upack( cfg_decoder_object_t *obj, uint8_t data )
{
//		static uint32_t s32ticktemp;
//		static uint8_t  u8data[4],u8idx=0;
//    switch( obj->step ) {
//      case 0:
//			if( data == 'R' ) {
//         obj->step = 1;
//			}
//			break;
//		case 1:
//			if( data == 'R' ){
//				obj->step = 2;
//			}else{
//				obj->step = 0;
//			}
//			break;
//		case 2:
//			if( data == 'R' ) {
//				obj->step = 3;
//			} else {
//				obj->step = 0;
//			}
//			break;
//		case 3:
//			if( data == ':' ) {
//				obj->step = 4;
//				s32ticktemp=g_current_tick;
//				u8idx=0;
//			} else {
//				obj->step = 0;
//			}
//			break;
//		case 4:
//			if( g_current_tick-s32ticktemp<10 ){
//				g_Distance=0;
//				g_Distance=data;
//				
//				s32ticktemp=g_current_tick;
//				u8data[u8idx]=data;
//				if( u8idx == 4 ){
//					g_Distance = (u8data[0]-'0')*1000+(u8data[2]-'0')*100+(u8data[3]-'0')*10+(u8data[4]-'0');
//					obj->step = 0;
//					uart_printf( 0,"d:%d \n\r ",g_Distance );
//					g_SonarGetTime = g_current_tick;
//				}
//				u8idx++;
//			}else{
//				obj->step = 0;
//			}
//       break;
////		case 5:
////			if( g_current_tick-s32ticktemp<10 ){
////				g_Distance += data<<8 ;
////				g_SonarGetTime = g_current_tick;
////				obj->step = 0;
////			}else{
////				obj->step = 0;
////				g_Distance=0;
////			}
////			break;
//		default:
//				obj->step = 0;
//    }
    return false;
}
#endif

