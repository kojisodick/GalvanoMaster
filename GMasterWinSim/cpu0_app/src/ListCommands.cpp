/*
 * ListCommands.cpp
 *
 *  Created on: 2018/11/14
 *      Author: imao_yusuke
 */

#include <Logger/Logger.h>
#include "Commands.h"
#include "SharedMemory0.h"
#include "ListBoxManager.h"

extern Logger         *gp_log;
extern SharedMemory0  *gp_sharedMemory;
extern ListBoxManager *gp_listManager;


//---------------------------------------------------------------------------------
/*!
 * lst_jump_abs(long xval, long yval)
 * Code : 0x80
 */
int4 Commands::command_0x80( CmdArgs &args )
{
	return m_skyWriting->calculate(m_mtnLinear.jumpAbs(args.si4[0], args.si4[1]));
}

//---------------------------------------------------------------------------------
/*!
 * lst_jump_rel(long dx, long dy)
 * Code : 0x81
 */
int4 Commands::command_0x81( CmdArgs &args )
{
	return m_skyWriting->calculate(m_mtnLinear.jumpRel(args.si4[0], args.si4[1]));
}

//---------------------------------------------------------------------------------
/*!
 * lst_mark_abs(long xval, long yval)
 * Code : 0x82
 */
int4 Commands::command_0x82( CmdArgs &args )
{
	return m_skyWriting->calculate(m_mtnLinear.markAbs(args.si4[0], args.si4[1]));
}

//---------------------------------------------------------------------------------
/*!
 * lst_mark_rel(long dx, long dy)
 * Code : 0x83
 */
int4 Commands::command_0x83( CmdArgs &args )
{
	return m_skyWriting->calculate(m_mtnLinear.markRel(args.si4[0], args.si4[1]));
}

//---------------------------------------------------------------------------------
/*!
 * lst_timed_jump_abs(long xval, long yval, double time)
 * Code : 0x84
 */
int4 Commands::command_0x84( CmdArgs &args )
{
	return m_skyWriting->calculate(m_mtnLinear.jumpAbs(args.si4[0], args.si4[1], args.rl8[2]));
}

//---------------------------------------------------------------------------------
/*!
 *  lst_timed_jump_rel(long dx, long dy, double time)
 * Code : 0x85
 */
int4 Commands::command_0x85( CmdArgs &args )
{
	return m_skyWriting->calculate(m_mtnLinear.jumpRel(args.si4[0], args.si4[1], args.rl8[2]));
}

//---------------------------------------------------------------------------------
/*!
 * lst_timed_mark_abs(long xval, long yval, double time)
 * Code : 0x86
 */
int4 Commands::command_0x86( CmdArgs &args )
{
	return m_skyWriting->calculate(m_mtnLinear.markAbs(args.si4[0], args.si4[1], args.rl8[2]));
}

//---------------------------------------------------------------------------------
/*!
 * lst_timed_mark_rel(long dx, long dy, double time)
 * Code : 0x87
 */
int4 Commands::command_0x87( CmdArgs &args )
{
	return m_skyWriting->calculate(m_mtnLinear.markRel(args.si4[0], args.si4[1], args.rl8[2]));
}

//---------------------------------------------------------------------------------
/*!
 * lst_jump_abs_3d(long xval, long yval, long zval)
 * Code : 0x88
 */
int4 Commands::command_0x88( CmdArgs &args )
{
	return m_skyWriting->calculate(m_mtnLinear.jumpAbs(args.si4[0], args.si4[1], args.si4[2]));
}

//---------------------------------------------------------------------------------
/*!
 * lst_jump_rel_3d(long dx, long dy, long dz)
 * Code : 0x89
 */
int4 Commands::command_0x89( CmdArgs &args )
{
	return m_skyWriting->calculate(m_mtnLinear.jumpRel(args.si4[0], args.si4[1], args.si4[2]));
}

//---------------------------------------------------------------------------------
/*!
 * lst_mark_abs_3d(long xval, long yval, long zval)
 * Code : 0x8a
 */
int4 Commands::command_0x8a( CmdArgs &args )
{
	return m_skyWriting->calculate(m_mtnLinear.markAbs(args.si4[0], args.si4[1], args.si4[2]));
}

//---------------------------------------------------------------------------------
/*!
 * lst_mark_rel_3d(long dx, long dy, long dz)
 * Code : 0x8b
 */
int4 Commands::command_0x8b( CmdArgs &args )
{
	return m_skyWriting->calculate(m_mtnLinear.markRel(args.si4[0], args.si4[1], args.si4[2]));
}

//---------------------------------------------------------------------------------
/*!
 * lst_timed_jump_abs_3d(long xval, long yval, long zval, double time)
 * Code : 0x8c
 */
int4 Commands::command_0x8c( CmdArgs &args )
{
	return m_skyWriting->calculate(m_mtnLinear.jumpAbs(args.si4[0], args.si4[1], args.si4[2], args.rl8[2]));
}

//---------------------------------------------------------------------------------
/*!
 * lst_timed_jump_rel_3d(long dx, long dy, long dz, double time)
 * Code : 0x8d
 */
int4 Commands::command_0x8d( CmdArgs &args )
{
	return m_skyWriting->calculate(m_mtnLinear.jumpRel(args.si4[0], args.si4[1], args.si4[2], args.rl8[2]));
}

//---------------------------------------------------------------------------------
/*!
 * lst_timed_mark_abs_3d(long xval, long yval, long zval, double time)
 * Code : 0x8e
 */
int4 Commands::command_0x8e( CmdArgs &args )
{
	return m_skyWriting->calculate(m_mtnLinear.markAbs(args.si4[0], args.si4[1], args.si4[2], args.rl8[2]));
}

//---------------------------------------------------------------------------------
/*!
 * lst_timed_mark_rel_3d(long dx, long dy, long dz, double time)
 * Code : 0x8f
 */
int4 Commands::command_0x8f( CmdArgs &args )
{
	return m_skyWriting->calculate(m_mtnLinear.markRel(args.si4[0], args.si4[1], args.si4[2], args.rl8[2]));
}

//---------------------------------------------------------------------------------
/*!
 * lst_arc_abs(long x, long y, double angle)
 * Code : 0x90
 */
int4 Commands::command_0x90( CmdArgs &args )
{
	return m_skyWriting->calculate(m_mtnArc.moveAbs(args.si4[0], args.si4[1], args.rl8[1]));
}

//---------------------------------------------------------------------------------
/*!
 * lst_arc_rel(long dx, long dy, double angle)
 * Code : 0x91
 */
int4 Commands::command_0x91( CmdArgs &args )
{
	return m_skyWriting->calculate(m_mtnArc.moveRel(args.si4[0], args.si4[1], args.rl8[1]));
}

//---------------------------------------------------------------------------------
/*!
 * lst_set_standby(unsigned long half_period, unsigned long pulse_width)
 * Code : 0x92
 */
int4 Commands::command_0x92( CmdArgs &args )
{
	return m_skyWriting->calculate(m_laser.setStandby(args.ui4[0], args.ui4[1]));
}

//---------------------------------------------------------------------------------
/*!
 * lst_set_laser_timing(unsigned long mode, unsigned long half_period, unsigned long pulse_width1, unsigned long pulse_width2)
 * Code : 0x93
 */
int4 Commands::command_0x93( CmdArgs &args )
{
	return m_skyWriting->calculate(m_laser.setTiming(args.ui4[0], args.ui4[1], args.ui4[2], args.ui4[3]));
}

//---------------------------------------------------------------------------------
/*!
 * lst_laser_on(unsigned long delay)
 * Code : 0x94
 */
int4 Commands::command_0x94( CmdArgs &args )
{
	return m_skyWriting->calculate(m_laser.signalOn(args.ui4[0]));
}

//---------------------------------------------------------------------------------
/*!
 * lst_laser_signal_on(void)
 * Code : 0x95
 */
int4 Commands::command_0x95( CmdArgs &args )
{
	return m_skyWriting->calculate(m_laser.signalOn());
}

//---------------------------------------------------------------------------------
/*!
 * lst_laser_signal_off(void)
 * Code : 0x96
 */
int4 Commands::command_0x96( CmdArgs &args )
{
	return m_skyWriting->calculate(m_laser.signalOff());
}

//---------------------------------------------------------------------------------
/*!
 * lst_set_laser_delays(long laser_on_delay, unsigned long laser_off_delay)
 * Code : 0x97
 */
int4 Commands::command_0x97( CmdArgs &args )
{
	return m_skyWriting->calculate(m_laser.setDelay(args.ui4[0], args.ui4[1]));
}

//---------------------------------------------------------------------------------
/*!
 * lst_set_scanner_delays(unsigned long jump_delay, unsigned long mark_delay, unsigned long polygon_delay)
 * Code : 0x98
 */
int4 Commands::command_0x98( CmdArgs &args )
{
	return m_skyWriting->calculate(m_scanner.setDelay(args.ui4[0], args.ui4[1], args.ui4[2]));
}

//---------------------------------------------------------------------------------
/*!
 * lst_set_jump_speed(double jump_speed)
 * Code : 0x99
 */
int4 Commands::command_0x99( CmdArgs &args )
{
	return m_skyWriting->calculate(m_scanner.setJumpSpeed(args.rl8[0]));
}

//---------------------------------------------------------------------------------
/*!
 * lst_set_mark_speed(double mark_speed)
 * Code : 0x9a
 */
int4 Commands::command_0x9a( CmdArgs &args )
{
	return m_skyWriting->calculate(m_scanner.setMarkSpeed(args.rl8[0]));
}

//---------------------------------------------------------------------------------
/*!
 * lst_set_circular(unsigned long amplitude, double frequency)
 * Code : 0x9b
 */
int4 Commands::command_0x9b( CmdArgs &args )
{
	return NoError;
}

//---------------------------------------------------------------------------------
/*!
 * lst_write_io_port(unsigned long value)
 * Code : 0x9c
 */
int4 Commands::command_0x9c( CmdArgs &args )
{
	return m_skyWriting->calculate(m_laserIO.setOutputValue(LaserIODout16, args.ui4[0]));
}

//---------------------------------------------------------------------------------
/*!
 * lst_write_4bit_port(unsigned long value)
 * Code : 0x9d
 */
int4 Commands::command_0x9d( CmdArgs &args )
{
	gp_log->debug("lst_write_4bit_port(%u) -> (PLStatus=0x%x)", args.ui4[0], gp_plReg->getRawValue(0, 0x40));

	return m_skyWriting->calculate(m_laserIO.setOutputValue(LaserIODout4, args.ui4[0]));
}

//---------------------------------------------------------------------------------
/*!
 * lst_write_12bit_port(unsigned long value)
 * Code : 0x9e
 */
int4 Commands::command_0x9e( CmdArgs &args )
{
	return m_skyWriting->calculate(m_laserIO.setOutputValue(LaserIODout12, args.ui4[0]));
}

//---------------------------------------------------------------------------------
/*!
 * lst_write_da_1(unsigned long value)
 * Code : 0x9f
 */
int4 Commands::command_0x9f( CmdArgs &args )
{
//	gp_log->debug("lst_write_da_1(%u)", args.ui4[0]);

	return m_skyWriting->calculate(m_laserIO.setOutputValue(LaserIODa1, args.ui4[0]));
}

//---------------------------------------------------------------------------------
/*!
 * lst_write_da_2(unsigned long value)
 * Code : 0xa0
 */
int4 Commands::command_0xa0( CmdArgs &args )
{
//	gp_log->debug("lst_write_da_2(%u)", args.ui4[0]);

	return m_skyWriting->calculate(m_laserIO.setOutputValue(LaserIODa2, args.ui4[0]));
}

//---------------------------------------------------------------------------------
/*!
 * lst_ramp_mode(unsigned long out12, unsigned long da1, unsigned long da2)
 * Code : 0xa1
 */
int4 Commands::command_0xa1( CmdArgs &args )
{
	return m_skyWriting->calculate(m_laserIO.enableRamp(args.ui4[0], args.ui4[1], args.ui4[2]));
}

//---------------------------------------------------------------------------------
/*!
 * lst_ramp_param(unsigned long port, unsigned long time, unsigned long delta, unsigned long value)
 * Code : 0xa2
 */
int4 Commands::command_0xa2( CmdArgs &args )
{
	return m_skyWriting->calculate(m_laserIO.setRamp(args.ui4[0], args.ui4[1], args.ui4[2], args.ui4[3]));
}

//---------------------------------------------------------------------------------
/*!
 * lst_ramp_end_mode(unsigned long out12, unsigned long da1, unsigned long da2)
 * Code : 0xa3
 */
int4 Commands::command_0xa3( CmdArgs &args )
{
	return m_skyWriting->calculate(m_laserIO.enableRampEnd(args.ui4[0], args.ui4[1], args.ui4[2]));
}

//---------------------------------------------------------------------------------
/*!
 * lst_ramp_end_param(unsigned long port, unsigned long time, unsigned long delta, unsigned long value, int4 tlen)
 * Code : 0xa4
 */
int4 Commands::command_0xa4( CmdArgs &args )
{
	return m_skyWriting->calculate(m_laserIO.setRampEnd(args.ui4[0], args.ui4[1], args.ui4[2], args.ui4[3], args.si4[4]));
}

//---------------------------------------------------------------------------------
/*!
 * lst_set_control_mode(unsigned long control_mode, unsigned long count_sel, unsigned long count_mode)
 * Code : 0xa5
 */
int4 Commands::command_0xa5( CmdArgs &args )
{
    return NoError;
}

//---------------------------------------------------------------------------------
/*!
 * lst_set_end_of_list(void)
 * Code : 0xa6
 */
int4 Commands::command_0xa6( CmdArgs &args )
{
	gp_log->debug("lst_set_end_of_list() -> (bwIndex=%u)", gp_plReg->getPsWriteIndex());

	if( !gp_listManager->isFinished() ){
		return NoError;
	}

	return m_skyWriting->calculate(m_mtnCtrl.end());
}

//---------------------------------------------------------------------------------
/*!
 * lst_set_wait(unsigned long wait_word)
 * Code : 0xa7
 */
int4 Commands::command_0xa7( CmdArgs &args )
{
	return m_skyWriting->calculate(m_mtnCtrl.pause(args.ui4[0]));
}

//---------------------------------------------------------------------------------
/*!
 * lst_list_nop(void)
 * Code : 0xa8
 */
int4 Commands::command_0xa8( CmdArgs &args )
{
	return m_skyWriting->calculate(m_mtnWait.wait(1));
}

//---------------------------------------------------------------------------------
/*!
 * lst_long_delay(unsigned long delay)
 * Code : 0xa9
 */
int4 Commands::command_0xa9( CmdArgs &args )
{
	return m_skyWriting->calculate(m_mtnWait.wait(args.ui4[0]));
}

//---------------------------------------------------------------------------------
/*!
 * lst_set_fly_x(double ScaleX)
 * Code : 0xaa
 */
int4 Commands::command_0xaa( CmdArgs &args )
{
    return NoError;
}

//---------------------------------------------------------------------------------
/*!
 * lst_set_fly_y(double ScaleY)
 * Code : 0xab
 */
int4 Commands::command_0xab( CmdArgs &args )
{
    return NoError;
}

//---------------------------------------------------------------------------------
/*!
 * lst_set_fly_xy(double ScaleX, double ScaleY)
 * Code : 0xac
 */
int4 Commands::command_0xac( CmdArgs &args )
{
    return NoError;
}

//---------------------------------------------------------------------------------
/*!
 * lst_fly_return(long x, long y)
 * Code : 0xad
 */
int4 Commands::command_0xad( CmdArgs &args )
{
    return NoError;
}

//---------------------------------------------------------------------------------
/*!
 * lst_set_trigger(unsigned long sw)
 * Code : 0xae
 */
int4 Commands::command_0xae( CmdArgs &args )
{
	gp_log->debug("lst_trigger(%u)", args.ui4[0]);

	ShmCommand cmd;
	cmd.code = (args.ui4[0] == 1) ? ShmCmdCodeWaitForStartOfGat : ShmCmdCodeWaitForEndOfGat;
	gp_sharedMemory->runCommand(cmd);

	return m_skyWriting->calculate(m_gathering.setTrigger(args.ui4[0]));
}

//---------------------------------------------------------------------------------
/*!
 * lst_save_and_restart_timer(void)
 * Code : 0xaf
 */
int4 Commands::command_0xaf( CmdArgs &args )
{
    return NoError;
}

//---------------------------------------------------------------------------------
/*!
 * lst_focus_shift(long shift)
 * Code : 0xb0
 */
int4 Commands::command_0xb0( CmdArgs &args )
{
	int4 err;
	bool skyWritingActive = m_skyWriting->isEnabled();

	if( skyWritingActive ) {
		m_skyWriting->disable();
		while( !m_skyWriting->isEmpty() ) {
			if( gp_plReg->getFreeSpaceInBlockArea() ) {
				m_skyWriting->run();
			}
			SLEEP_USEC(100);
		}
	}

	m_mtnLinear.setOffset(Z, args.si4[0]);
	err = m_mtnLinear.jumpRel(0, 0, 0, 0.0)->run();

	if( skyWritingActive ) {
		m_skyWriting->enable();
	}

	return err;
}

//---------------------------------------------------------------------------------
/*!
 * lst_set_vsc_param(unsigned long enabled, unsigned long wait, unsigned long pre, unsigned long post)
 * Code : 0xb1
 */
int4 Commands::command_0xb1( CmdArgs &args )
{
    return m_skyWriting->setParameter(args.ui4[0], args.ui4[1], args.ui4[2], args.ui4[3]);
}
//---------------------------------------------------------------------------------
/*!
 * lst_set_vpc_param(unsigned long enabled, double mfreq, unsigned long amb, double vamp)
 * Code : 0xb2
 */
int4 Commands::command_0xb2( CmdArgs &args )
{
    return NoError;
}

//---------------------------------------------------------------------------------
/*!
 * lst_set_vsc_mode(unsigned long mode)
 */
int4 Commands::command_0xb3( CmdArgs &args )
{
	uint4 mode = args.ui4[0];

	if( mode == 0) {
		m_skyWriting = &m_skyWritingMode0;
	}
	else if( mode == 1 ) {
		m_skyWriting = &m_skyWritingMode1;
	}
	else if( mode == 2 ) {
		m_skyWriting = &m_skyWritingMode2;
	}
	else if( mode == 3 ) {
		m_skyWriting = &m_skyWritingMode3;
	}
	else {
		m_skyWriting->disable();
		return -1; //FIXME
	}

	m_skyWriting->enable();

    return NoError;
}
//---------------------------------------------------------------------------------
/*!
 * lst_set_vsc_limit(double limit)
 */
int4 Commands::command_0xb4( CmdArgs &args )
{
	m_skyWritingMode3.setLimit(args.rl8[0]);
    return NoError;
}
//---------------------------------------------------------------------------------
/*!
 */
int4 Commands::command_0xb5( CmdArgs &args )
{
    return NoError;
}
//---------------------------------------------------------------------------------
/*!
 */
int4 Commands::command_0xb6( CmdArgs &args )
{
    return NoError;
}
//---------------------------------------------------------------------------------
/*!
 */
int4 Commands::command_0xb7( CmdArgs &args )
{
    return NoError;
}
