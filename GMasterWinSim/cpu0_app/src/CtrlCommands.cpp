/*
 * CtrlCommands.cpp
 *
 *  Created on: 2018/11/14
 *      Author: imao_yusuke
 */

#include "Commands.h"
#include "SharedMemory0.h"
#include "ListBoxManager.h"
#include "Server.h"

#include <Logger/Logger.h>
extern Logger *gp_log;

#define CHECK_LIST_RUNNING \
	if( gp_listManager->isRunning() ) { \
		return ListIsRunning; \
	}


extern SharedMemory0  *gp_sharedMemory;
extern ListBoxManager *gp_listManager;
extern Server         *gp_server;

//---------------------------------------------------------------------------------
/*
 * ctr_select(unsigned long CardNo)
 * Code : 0x01
 */
int4 Commands::command_0x01( CmdArgs &args )
{
	CHECK_LIST_RUNNING

	gp_log->debug("ctr_select(%u)", args.ui4[0]);

	if( gp_plReg->getPlStatus() != PlStsBoot) {
		return NoError;
	}

	uint4 retryCnt = 0;
	while( 1 ) {
		if( retryCnt >= 3 ) {
			return PLInitFailed;
		}

		int4 err = gp_plReg->setPLInitRequest();
		if( !err ) {
			break;
		}

		++retryCnt;
		SLEEP_USEC(100);
	}

	return NoError;
}

//---------------------------------------------------------------------------------
/*
 * ctr_load_program_file(const char* FileName)
 * Code : 0x02
 */
int4 Commands::command_0x02( CmdArgs &args )
{
	CHECK_LIST_RUNNING

	return NoError;
}

//---------------------------------------------------------------------------------
/*
 * ctr_load_correction_file(const char* FileName, double kx, double ky, double phi, double x_offset, double y_offset)
 * Code : 0x03
 */
int4 Commands::command_0x03( CmdArgs &args )
{
	CHECK_LIST_RUNNING

	gp_log->debug("ctr_load_correction_file");

#if 1
	int4 *crcDataPtr = (int4 *)&gp_server->buffer.data;
	int4 err = gp_plReg->setCorrectionData(crcDataPtr);

	if( err ) {
		return err;
	}

	real4 *paramPtr = (real4 *)(crcDataPtr + 128*128*MAX_NUM_OF_AXIS);

	err = m_opticalSys.setWeight(paramPtr, paramPtr[10])->run(true);
	if( err ) {
		return err;
	}

	err = m_opticalSys.setParameter(paramPtr[10], paramPtr[12], paramPtr[13], paramPtr[14])->run(true);
	if( err ) {
		return err;
	}

	for( uint4 i=0; i<2; ++i ) {
		m_mtnLinear.setMagnification(i, paramPtr[25+i]);
		m_mtnLinear.setOffset(i, (int4)paramPtr[28+i]);
	}
	m_mtnLinear.setRotationAngle(paramPtr[27]);

#endif

	return NoError;
}

//---------------------------------------------------------------------------------
/*
 * ctr_count_cards(unsigned long *num)
 * Code : 0x04
 */
int4 Commands::command_0x04( CmdArgs &args )
{
    return NoError;
}

//---------------------------------------------------------------------------------
/*
 * ctr_get_dll_version(unsigned long *version)
 * Code : 0x05
 */
int4 Commands::command_0x05( CmdArgs &args )
{
    return NoError;
}

//---------------------------------------------------------------------------------
/*
 * ctr_get_hex_version(unsigned long *version)
 * Code : 0x06
 */
int4 Commands::command_0x06( CmdArgs &args )
{
    args.ui4[0] = gp_plReg->getPlVersion();

    return NoError;
}

//---------------------------------------------------------------------------------
/*
 * ctr_get_version(unsigned long *version)
 * Code : 0x07
 */
int4 Commands::command_0x07( CmdArgs &args )
{
    args.ui1[0] = PS_VERSION.build;
    args.ui1[1] = PS_VERSION.revision;
    args.ui1[2] = PS_VERSION.minor;
    args.ui1[3] = PS_VERSION.major;

    args.ui4[1] = gp_plReg->getPlVersion();

    args.ui4[2] = gp_plReg->getDriverVersion();

    args.ui1[12] = BIN_VERSION.build;
    args.ui1[13] = BIN_VERSION.revision;
    args.ui1[14] = BIN_VERSION.minor;
    args.ui1[15] = BIN_VERSION.major;

    args.ui1[16] = RELEASE_DATE.day;
    args.ui1[17] = RELEASE_DATE.month;
    args.ui1[18] = RELEASE_DATE.year;
    args.ui1[19] = 0;


    return NoError;
}

//---------------------------------------------------------------------------------
/*
 * ctr_set_laser_mode(unsigned long mode, unsigned long gt_pol, unsigned long p1_pol, unsigned long p2_pol)
 * Code : 0x08
 */
int4 Commands::command_0x08( CmdArgs &args )
{
	CHECK_LIST_RUNNING

	gp_log->debug("ctr_set_laser_mode(%u,%u,%u,%u)", args.ui4[0], args.ui4[1], args.ui4[2], args.ui4[3]);

	return m_laser.setMode(args.ui4[0], args.ui4[1], args.ui4[2], args.ui4[3])->run(true);
}

//---------------------------------------------------------------------------------
/*
 * ctr_set_laser_timing(unsigned long mode, unsigned long half_period, unsigned long pulse_width1, unsigned long pulse_width2)
 * Code : 0x09
 */
int4 Commands::command_0x09( CmdArgs &args )
{
	CHECK_LIST_RUNNING

	gp_log->debug("ctr_set_laser_timing(%u,%x,%x,%x)", args.ui4[0], args.ui4[1], args.ui4[2], args.ui4[3]);

	return m_laser.setTiming(args.ui4[0], args.ui4[1], args.ui4[2], args.ui4[3])->run(true);
}

//---------------------------------------------------------------------------------
/*
 * ctr_disable_laser(void)
 * Code : 0x0a
 */
int4 Commands::command_0x0a( CmdArgs &args )
{
	CHECK_LIST_RUNNING

	m_laser.disable()->run(true);

	SLEEP_USEC(10);
	gp_log->debug("ctr_disable_laser() -> (PLStatus=0x%x)", gp_plReg->getRawValue(0, 0x40));

	return NoError;
}

//---------------------------------------------------------------------------------
/*
 * ctr_enable_laser(void)
 * Code : 0x0b
 */
int4 Commands::command_0x0b( CmdArgs &args )
{
	CHECK_LIST_RUNNING

	m_laser.enable()->run(true);

	SLEEP_USEC(10);
	gp_log->debug("ctr_enable_laser() -> (PLStatus=0x%x)", gp_plReg->getRawValue(0, 0x40));

	return NoError;
}

//---------------------------------------------------------------------------------
/*
 * ctr_set_standby(unsigned long half_period, unsigned long pulse_width)
 * Code : 0x0c
 */
int4 Commands::command_0x0c( CmdArgs &args )
{
	CHECK_LIST_RUNNING

	return m_laser.setStandby(args.ui4[0], args.ui4[1])->run(true);
}

//---------------------------------------------------------------------------------
/*
 * ctr_laser_signal_on(void)
 * Code : 0x0d
 */
int4 Commands::command_0x0d( CmdArgs &args )
{
	CHECK_LIST_RUNNING

	return m_laser.signalOn()->run(true);
}

//---------------------------------------------------------------------------------
/*
 * ctr_laser_signal_off(void)
 * Code : 0x0e
 */
int4 Commands::command_0x0e( CmdArgs &args )
{
	CHECK_LIST_RUNNING
	return m_laser.signalOff()->run(true);
}

//---------------------------------------------------------------------------------
/*
 * ctr_set_laser_delays(unsigned long laser_on_delay, unsigned long laser_off_delay)
 * Code : 0x0f
 */
int4 Commands::command_0x0f( CmdArgs &args )
{
	CHECK_LIST_RUNNING

	return m_laser.setDelay(args.ui4[0], args.ui4[1])->run(true);

}

//---------------------------------------------------------------------------------
/*
 * ctr_set_scanner_delays(unsigned long jump_delay, unsigned long mark_delay, unsigned long polygon_delay)
 * Code : 0x10
 */
int4 Commands::command_0x10( CmdArgs &args )
{
	CHECK_LIST_RUNNING

	return m_scanner.setDelay(args.ui4[0], args.ui4[1], args.ui4[2])->run(true);
}

//---------------------------------------------------------------------------------
/*
 * ctr_set_jump_speed(double jump_speed)
 * Code : 0x11
 */
int4 Commands::command_0x11( CmdArgs &args )
{
	CHECK_LIST_RUNNING

	return m_scanner.setJumpSpeed(args.rl8[0])->run(true);
}

//---------------------------------------------------------------------------------
/*
 * ctr_set_mark_speed(double mark_speed)
 * Code : 0x12
 */
int4 Commands::command_0x12( CmdArgs &args )
{
	CHECK_LIST_RUNNING

	return m_scanner.setMarkSpeed(args.rl8[0])->run(true);
}

//---------------------------------------------------------------------------------
/*
 * ctr_goto_xy(long xpos, long ypos)
 * Code : 0x13
 */
int4 Commands::command_0x13( CmdArgs &args )
{
	CHECK_LIST_RUNNING

	return m_mtnLinear.jumpAbs(args.si4[0], args.si4[1])->run(true);
}

//---------------------------------------------------------------------------------
/*
 * ctr_get_xy_pos(long *xpos, long *ypos)
 * Code : 0x14
 */
int4 Commands::command_0x14( CmdArgs &args )
{
	args.si4[0] = m_mtnLinear.endPos[X];
	args.si4[1] = m_mtnLinear.endPos[Y];
    return NoError;
}

//---------------------------------------------------------------------------------
/*
 * ctr_goto_xyz(long xpos, long ypos, long zpos)
 * Code : 0x15
 */
int4 Commands::command_0x15( CmdArgs &args )
{
	CHECK_LIST_RUNNING

	return m_mtnLinear.jumpAbs(args.si4[0], args.si4[1], args.si4[2])->run(true);
}

//---------------------------------------------------------------------------------
/*
 * ctr_get_xyz_pos(long *xpos, long *ypos, long *zpos)
 * Code : 0x16
 */
int4 Commands::command_0x16( CmdArgs &args )
{
	args.si4[0] = m_mtnLinear.endPos[X];
	args.si4[1] = m_mtnLinear.endPos[Y];
	args.si4[2] = m_mtnLinear.endPos[Z];

    return NoError;
}

//---------------------------------------------------------------------------------
/*
 * ctr_set_port_enb(unsigned long out12, unsigned long out16, unsigned long out4, unsigned long da)
 * Code : 0x17
 */
int4 Commands::command_0x17( CmdArgs &args )
{
	CHECK_LIST_RUNNING

	return m_laserIO.enableIO(args.ui4[0], args.ui4[1], args.ui4[2], args.ui4[3])->run(true);
}

//---------------------------------------------------------------------------------
/*
 * ctr_set_port_default(unsigned long port, unsigned long value)
 * Code : 0x18
 */
int4 Commands::command_0x18( CmdArgs &args )
{
	CHECK_LIST_RUNNING

	return m_laserIO.setDefaultValue(args.ui4[0], args.ui4[1])->run(true);
}

//---------------------------------------------------------------------------------
/*
 * ctr_read_io_port(unsigned long * iodata)
 * Code : 0x19
 */
int4 Commands::command_0x19( CmdArgs &args )
{
	args.ui4[0] = gp_plReg->getLaserIOValue(LaserPortDin16);

	return NoError;
}

//---------------------------------------------------------------------------------
/*
 * ctr_write_io_port(unsigned long value)
 * Code : 0x1a
 */
int4 Commands::command_0x1a( CmdArgs &args )
{
	CHECK_LIST_RUNNING

	return m_laserIO.setOutputValue(LaserIODout16, args.ui4[0])->run(true);
}

//---------------------------------------------------------------------------------
/*
 * ctr_read_4bit_port(unsigned long * iodata)
 * Code : 0x1b
 */
int4 Commands::command_0x1b( CmdArgs &args )
{
	args.ui4[0] = gp_plReg->getLaserIOValue(LaserPortDin4);

    return NoError;
}

//---------------------------------------------------------------------------------
/*
 * ctr_write_4bit_port(unsigned long value)
 * Code : 0x1c
 */
int4 Commands::command_0x1c( CmdArgs &args )
{
	CHECK_LIST_RUNNING

	gp_log->debug("ctr_write_4bit_port(%u)", args.ui4[0]);

	return m_laserIO.setOutputValue(LaserIODout4, args.ui4[0])->run(true);
}

//---------------------------------------------------------------------------------
/*
 * ctr_write_12bit_port(unsigned long value)
 * Code : 0x1d
 */
int4 Commands::command_0x1d( CmdArgs &args )
{
	CHECK_LIST_RUNNING

	return m_laserIO.setOutputValue(LaserIODout12, args.ui4[0])->run(true);
}

//---------------------------------------------------------------------------------
/*
 * ctr_write_da_1(unsigned long value)
 * Code : 0x1e
 */
int4 Commands::command_0x1e( CmdArgs &args )
{
	CHECK_LIST_RUNNING

//	gp_log->debug("ctr_write_da_1(%u)", args.ui4[0]);

	return m_laserIO.setOutputValue(LaserIODa1, args.ui4[0])->run(true);
}

//---------------------------------------------------------------------------------
/*
 * ctr_write_da_2(unsigned long value)
 * Code : 0x1f
 */
int4 Commands::command_0x1f( CmdArgs &args )
{
	CHECK_LIST_RUNNING

//	gp_log->debug("ctr_write_da_2(%u)", args.ui4[0]);

	return m_laserIO.setOutputValue(LaserIODa2, args.ui4[0])->run(true);
}

//---------------------------------------------------------------------------------
/*
 * ctr_ramp_mode(unsigned long out12, unsigned long da1, unsigned long da2)
 * Code : 0x20
 */
int4 Commands::command_0x20( CmdArgs &args )
{
	CHECK_LIST_RUNNING

	return m_laserIO.enableRamp(args.ui4[0], args.ui4[1], args.ui4[2])->run(true);
}

//---------------------------------------------------------------------------------
/*
 * ctr_ramp_param(unsigned long port, unsigned long time, unsigned long delta, unsigned long value)
 * Code : 0x21
 */
int4 Commands::command_0x21( CmdArgs &args )
{
	CHECK_LIST_RUNNING

	return m_laserIO.setRamp(args.ui4[0], args.ui4[1], args.ui4[2], args.ui4[3])->run(true);
}

//---------------------------------------------------------------------------------
/*
 * ctr_ramp_end_mode ( unsigned long out12, unsigned long da1, unsigned long da2 )
 * Code : 0x22
 */
int4 Commands::command_0x22( CmdArgs &args )
{
	CHECK_LIST_RUNNING

	return m_laserIO.enableRampEnd(args.ui4[0], args.ui4[1], args.ui4[2])->run(true);
}

//---------------------------------------------------------------------------------
/*
 * ctr_ramp_end_param ( unsigned long port, unsigned long time, unsigned long delta, unsigned long value, unsigned long tlen )
 * Code : 0x23
 */
int4 Commands::command_0x23( CmdArgs &args )
{
	CHECK_LIST_RUNNING

	return m_laserIO.setRampEnd(args.ui4[0], args.ui4[1], args.ui4[2], args.ui4[3], args.si4[4])->run(true);
}

//---------------------------------------------------------------------------------
/*
 * ctr_set_control_mode(unsigned long control_mode, unsigned long count_sel, unsigned long count_mode)
 * Code : 0x24
 */
int4 Commands::command_0x24( CmdArgs &args )
{
	CHECK_LIST_RUNNING

	return NoError;
}

//---------------------------------------------------------------------------------
/*
 * ctr_select_list(unsigned long list)
 * Code : 0x25
 */
int4 Commands::command_0x25( CmdArgs &args )
{
	CHECK_LIST_RUNNING

    ListBoxCh ch = (ListBoxCh)(args.ui4[0] - 1); 

	gp_listManager->selectRunningList(ch);

    return NoError;
}

//---------------------------------------------------------------------------------
/*
 * ctr_get_startstop_info(unsigned long * info)
 * Code : 0x26
 */
int4 Commands::command_0x26( CmdArgs &args )
{
    return NoError;
}

//---------------------------------------------------------------------------------
/*
 * ctr_set_ext_start_delay(long Delay)
 * Code : 0x27
 */
int4 Commands::command_0x27( CmdArgs &args )
{
	CHECK_LIST_RUNNING

    return NoError;
}

//---------------------------------------------------------------------------------
/*
 * ctr_get_delay_count(long * count)
 * Code : 0x28
 */
int4 Commands::command_0x28( CmdArgs &args )
{
    return NoError;
}

//---------------------------------------------------------------------------------
/*
 * ctr_set_start_list(unsigned long list_no)
 * Code : 0x29
 */
int4 Commands::command_0x29( CmdArgs &args )
{
	gp_log->debug("ctr_set_start_list(list_no=%u)", args.ui4[0]);

	ListBoxCh ch = (ListBoxCh)(args.ui4[0] - 1);

    return gp_listManager->startWriting(ch);
}

//---------------------------------------------------------------------------------
/*
 * ctr_execute_list(unsigned long list_no)
 * Code : 0x2a
 */
int4 Commands::command_0x2a( CmdArgs &args )
{
	CHECK_LIST_RUNNING

	gp_log->debug("ctr_execute_list(list_no=%u)", args.ui4[0]);

    ListBoxCh ch = (ListBoxCh)(args.ui4[0] - 1); 

	int err = m_skyWriting->calculate(m_mtnCtrl.begin());

	err = gp_listManager->startRunning(ch);

	if( err ) {
		gp_log->error(" -> startRunning error. (%d)", err);
		return err;
	}

#ifdef __arm__
	err = this->runListCommands();
	if( err ) {
		gp_log->error(" -> runListCommands error. (%d)", err);
		return err;
	}

    err = gp_plReg->setEnterRequest();
	if( err ) {
		gp_log->error(" -> setEnterRequest error. (%d)", err);
		return err;
	}

	uint4 counter = 0;
    while( 1 ) {
        if( gp_plReg->getPlStatus() == PlStsBusy) {
        	break;
        }

    	if( counter++ > 1000) {
    		gp_log->error("-> timeout error. (PLStatus=0x%x)", gp_plReg->getPlStatus());
    		gp_listManager->stopRunning();
    		gp_plReg->setStopRequest();
    		return PLIsBusy;
    	}
		SLEEP_USEC(10);
    }
#endif

	gp_log->debug(" -> Start list motion!");

    return NoError;
}

//---------------------------------------------------------------------------------
/*
 * ctr_execute_at_pointer(unsigned long pointer)
 * Code : 0x2b
 */
int4 Commands::command_0x2b( CmdArgs &args )
{
	CHECK_LIST_RUNNING

	m_skyWriting->calculate(m_mtnCtrl.begin());

	gp_listManager->startRunningAtPoint(args.ui4[0]);

	this->runListCommands();

    return gp_plReg->setEnterRequest();
}

//---------------------------------------------------------------------------------
/*
 * ctr_stop_execution(void)
 * Code : 0x2c
 */
int4 Commands::command_0x2c( CmdArgs &args )
{
	gp_log->debug("ctr_stop_execution");
	gp_listManager->stopRunning();
    gp_plReg->setStopRequest();
    m_skyWriting->clear();

    uint4 status = gp_plReg->getPlStatus();
    uint4 counter = 0;
    while( status != PlStsError ) {
    	if( counter > 1000 ){
        	gp_log->error("-> PL_STOP timeout");
    		return -1;
    	}
		SLEEP_USEC(10);
    	counter++;
    	status = gp_plReg->getPlStatus();
    }

    gp_plReg->setAckRequest();

    while( status != PlStsActive ) {
    	if( counter > 1000 ){
        	gp_log->error("-> PL_ACK timeout");
    		return -2;
    	}
		SLEEP_USEC(10);
    	counter++;
    	status = gp_plReg->getPlStatus();
    }

	gp_log->debug("-> successful. (time=%u usec)", counter*10);

    return NoError;
}

//---------------------------------------------------------------------------------
/*
 * ctr_stop_list(void)
 * Code : 0x2d
 */
int4 Commands::command_0x2d( CmdArgs &args )
{
	gp_log->debug("ctr_stop_list");

	return gp_plReg->setHaltRequest();
}

//---------------------------------------------------------------------------------
/*
 * ctr_restart_list(void)
 * Code : 0x2e
 */
int4 Commands::command_0x2e( CmdArgs &args )
{
	gp_log->debug("ctr_restart_list");

	return gp_plReg->setEnterRequest();
}

//---------------------------------------------------------------------------------
/*
 * ctr_get_status(unsigned long *busy, unsigned long *position)
 * Code : 0x2f
 */
int4 Commands::command_0x2f( CmdArgs &args )
{
	args.ui4[0] = (gp_plReg->getPlStatus()==PlStsBusy) ? 1 : 0;

	uint4 ch = gp_listManager->getReadCh();

	args.ui4[1] = gp_listManager->getReadIndex() + 1000000 * ch;

    return NoError;
}

//---------------------------------------------------------------------------------
/*
 * ctr_read_status(void)
 * Code : 0x30
 */
int4 Commands::command_0x30( CmdArgs &args )
{
#if 1
	ListBoxManager *manager = gp_listManager;

	union Status  // format is determined by Canon
	{
		uint4 word;
		struct {
			uint4 list1_loading : 1;
			uint4 list2_loading : 1;
			uint4 list1_ready   : 1;
			uint4 list2_ready   : 1;
			uint4 list1_running : 1;
			uint4 list2_running : 1;
			uint4 fixedVal0     : 2;
			uint4 fixedVal1     : 8;
		} bit;
	} status;

	status.word = manager->getStatusVal();

	uint4 plStatus = gp_plReg->getPlStatus();
	if( plStatus == PlStsBusy ) {
		if( manager->getReadCh() == ListBox1 ) {
			status.bit.list1_ready   = 0;
			status.bit.list1_running = 1;
		}
		else {
			status.bit.list2_ready   = 0;
			status.bit.list2_running = 1;
		}
	}
	else if (plStatus == PlStsError ) {
		status.word = 0;
	}

	args.ui4[0] = status.word;
#else
	args.ui4[0] = gp_listManager->getStatusVal();
#endif

//	if( args.ui4[0] != 0xFF10 ) {
//		gp_log->debug("ctr_read_status(%x)", args.ui4[0]);
//	}
    return NoError;
}

//---------------------------------------------------------------------------------
/*
 * ctr_get_wait_status(void)
 * Code : 0x31
 */
int4 Commands::command_0x31( CmdArgs &args )
{
    //args.ui4[0] = gp_plReg->;
    return NoError;
}

//---------------------------------------------------------------------------------
/*
 * ctr_release_wait(void)
 * Code : 0x32
 */
int4 Commands::command_0x32( CmdArgs &args )
{
	gp_log->debug("ctr_release_wait");

	return gp_plReg->setEnterRequest();
}

//---------------------------------------------------------------------------------
/*
 * ctr_auto_change(void)
 * Code : 0x33
 */
int4 Commands::command_0x33( CmdArgs &args )
{
    gp_listManager->setAutoChange(true);

    return NoError;
}

//---------------------------------------------------------------------------------
/*
 * ctr_start_loop(void)
 * Code : 0x34
 */
int4 Commands::command_0x34( CmdArgs &args )
{
	gp_listManager->setLoop(true);

	return NoError;
}

//---------------------------------------------------------------------------------
/*
 * ctr_quit_loop(void)
 * Code : 0x35
 */
int4 Commands::command_0x35( CmdArgs &args )
{
	gp_listManager->setLoop(false);

	return NoError;
}

//---------------------------------------------------------------------------------
/*
 * ctr_set_input_pointer(unsigned long pointer)
 * Code : 0x36
 */
int4 Commands::command_0x36( CmdArgs &args )
{
	return gp_listManager->startWritingAtPoint(args.ui4[0]);
}

//---------------------------------------------------------------------------------
/*
 * ctr_get_input_pointer(unsigned long * pointer)
 * Code : 0x37
 */
int4 Commands::command_0x37( CmdArgs &args )
{
	args.ui4[0] = gp_listManager->getWriteIndex();
    return NoError;
}

//---------------------------------------------------------------------------------
/*
 * ctr_simulate_encoder(unsigned long Enc_mode)
 * Code : 0x38
 */
int4 Commands::command_0x38( CmdArgs &args )
{
    return NoError;
}

//---------------------------------------------------------------------------------
/*
 * ctr_get_encoder(unsigned long *Encoder0, unsigned long *Encoder1)
 * Code : 0x39
 */
int4 Commands::command_0x39( CmdArgs &args )
{
    return NoError;
}

//---------------------------------------------------------------------------------
/*
 * ctr_get_marking_info(unsigned long * info)
 * Code : 0x3a
 */
int4 Commands::command_0x3a( CmdArgs &args )
{
    return NoError;
}

//---------------------------------------------------------------------------------
/*
 * ctr_xy2input_delay(unsigned long delay)
 * Code : 0x3b
 */
int4 Commands::command_0x3b( CmdArgs &args )
{
    return NoError;
}

//---------------------------------------------------------------------------------
/*
 * ctr_get_value(unsigned long signal, long * value)
 * Code : 0x3c
 */
int4 Commands::command_0x3c( CmdArgs &args )
{
	enum GetValueItems: uint4
	{
		BoardStatus1            = 0,
		BoardStatus2            = 1,
		CommandPosition_X_Pulse = 2,
		CommandPosition_Y_Pulse = 4,
		CommandPosition_Z_Pulse = 6,
		ScannerDriverStatus1    = 8,
		ScannerDriverStatus2    = 10,
		ScannerDriverStatus3    = 12,
		Dout4OutputValue        = 14,
		Dout16OutputValue       = 15,
		Dout12OutputValue       = 16,
		Da1OutputValue          = 17,
		Da2OutputValue          = 18,
		DIn4InputValue          = 19,
		DIn16InputValue         = 20,
		Encoder1InputValue      = 21,
		Encoder2InputValue      = 22,
		ExternalStartDelayCounter = 24,
	};

	switch( args.ui4[0] ) {
	    case BoardStatus1:
    		args.ui4[1] = gp_plReg->getRawValue(0, 0x40);
	    	break;
	    case BoardStatus2:
    		args.ui4[1] = 0;
	    	break;
	    case CommandPosition_X_Pulse:
    		args.ui4[1] = gp_plReg->getCmdPosPulse(X);
	    	break;
	    case CommandPosition_Y_Pulse:
    		args.ui4[1] = gp_plReg->getCmdPosPulse(Y);
	    	break;
	    case CommandPosition_Z_Pulse:
    		args.ui4[1] = gp_plReg->getCmdPosPulse(Z);
	    	break;
	    case ScannerDriverStatus1:
    		args.ui4[1] = gp_plReg->getDriverStatus(X);
	    	break;
	    case ScannerDriverStatus2:
    		args.ui4[1] = gp_plReg->getDriverStatus(Y);
	    	break;
	    case ScannerDriverStatus3:
    		args.ui4[1] = gp_plReg->getDriverStatus(Z);
	    	break;
	    case Dout4OutputValue:
    		args.ui4[1] = gp_plReg->getLaserIOValue(LaserPortDout4);
	    	break;
	    case Dout16OutputValue:
    		args.ui4[1] = gp_plReg->getLaserIOValue(LaserPortDout16);
	    	break;
	    case Dout12OutputValue:
    		args.ui4[1] = gp_plReg->getLaserIOValue(LaserPortDout12);
	    	break;
	    case Da1OutputValue:
    		args.ui4[1] = gp_plReg->getLaserIOValue(LaserPortDA1);
	    	break;
	    case Da2OutputValue:
    		args.ui4[1] = gp_plReg->getLaserIOValue(LaserPortDA2);
	    	break;
	    case DIn4InputValue:
    		args.ui4[1] = gp_plReg->getLaserIOValue(LaserPortDin4);
	    	break;
	    case DIn16InputValue:
    		args.ui4[1] = gp_plReg->getLaserIOValue(LaserPortDin16);
	    	break;
	    case Encoder1InputValue:
    		args.ui4[1] = 0;
	    	break;
	    case Encoder2InputValue:
    		args.ui4[1] = 0;
	    	break;
	    case ExternalStartDelayCounter:
    		args.ui4[1] = 0;
	    	break;
	    default:
    		args.ui4[1] = 0;
	    	break;
	}

//	gp_log->debug("ctr_get_value(%u, %x)", args.ui4[0], args.ui4[1]);
    return NoError;
}

//---------------------------------------------------------------------------------
/*
 * ctr_set_measurement_signal(unsigned long port, unsigned long signal)
 * Code : 0x3d
 */
int4 Commands::command_0x3d( CmdArgs &args )
{
//	gp_log->debug("ctr_set_measurement_signal(%u, %x)", args.ui4[0], args.ui4[1]);

	ShmCommand cmd;
	cmd.code = ShmCmdCodeSetGatSignal;
	cmd.args.ui4[0] = args.ui4[0];
	cmd.args.ui4[1] = args.ui4[1];

	gp_sharedMemory->runCommand(cmd);

    return NoError;
}

//---------------------------------------------------------------------------------
/*
 * ctr_set_measurement_config(unsigned long period, unsigned long buf_num)
 * Code : 0x3e
 */
int4 Commands::command_0x3e( CmdArgs &args )
{
	ShmCommand cmd;
	cmd.code = ShmCmdCodeSetGatConfig;
	cmd.args.ui4[0] = args.ui4[0];
	cmd.args.ui4[1] = args.ui4[1];

	gp_sharedMemory->runCommand(cmd);

    return NoError;
}

//---------------------------------------------------------------------------------
/*
 * ctr_set_trigger(unsigned long sw)
 * Code : 0x3f
 */
int4 Commands::command_0x3f( CmdArgs &args )
{
	ShmCommand cmd;
	cmd.code = (args.ui4[0] == 1) ? ShmCmdCodeStartGat : ShmCmdCodeEndGat;
	gp_sharedMemory->runCommand(cmd);

	return NoError;
}

//---------------------------------------------------------------------------------
/*
 * ctr_measurement_status(unsigned long *busy, unsigned long *full, unsigned long *position)
 * Code : 0x40
 */
int4 Commands::command_0x40( CmdArgs &args )
{
	ShmCommand cmd;
	cmd.code = ShmCmdCodeGetGatStatus;

	gp_sharedMemory->runCommand(cmd);

	args.ui4[0] = cmd.args.ui4[0];
	args.ui4[1] = cmd.args.ui4[1];
	args.ui4[2] = cmd.args.ui4[2];

    return NoError;
}

//---------------------------------------------------------------------------------
/*
 * ctr_get_waveform(unsigned long port, signed long *memptr)
 * Code : 0x41
 */
int4 Commands::command_0x41( CmdArgs &args )
{
//	gp_log->debug("ctr_get_waveform(%u)", args.ui4[0]);

	gp_server->buffer.clear();

	ShmCommand cmd;
	cmd.code = ShmCmdCodeUploadGatData;
	cmd.args.ui4[0] = args.ui4[0];

	gp_sharedMemory->sendCmdRequest(cmd);

	int4 res = gp_sharedMemory->downloadData(gp_server->buffer.data, sizeof(gp_server->buffer.data));
	if( res != NoError ) {
		gp_log->error("ctr_get_waveform(port=%u) -> Failed to download gathering data.", args.ui4[0]);
	}

	gp_server->buffer.dataLen = gp_sharedMemory->getTotalDataLen();

	gp_sharedMemory->recvCmdResponse(cmd);

//	uint4  len = gp_server->buffer.dataLen;
//	uint4 *ptr = (uint4 *)gp_server->buffer.data;
//	while( len > 0 ) {
//		xil_printf("%u\r\n", *ptr);
//		++ptr;
//		len -= 4;
//	}

    return NoError;
}

//---------------------------------------------------------------------------------
/*
 * ctr_get_waveform_ex(unsigned long start, unsigned long stop, signed long *memptr)
 * Code : 0x42
 */
int4 Commands::command_0x42( CmdArgs &args )
{
	gp_server->buffer.clear();

	ShmCommand cmd;
	cmd.code = ShmCmdCodeUploadGatData;
	cmd.args.ui4[0] = args.ui4[0];

	gp_sharedMemory->runCommand(cmd);

	SLEEP_USEC(1);

	int4 res = gp_sharedMemory->downloadData(gp_server->buffer.data, sizeof(gp_server->buffer.data));
	if( res != NoError ) {
		gp_log->error("ctr_get_waveform_ex(port=%u) -> Failed to download gathering data.", args.ui4[0]);
	}

	gp_server->buffer.dataLen = gp_sharedMemory->getTotalDataLen();

    return NoError;
}

//---------------------------------------------------------------------------------
/*
 * ctr_get_time(double * time)
 * Code : 0x43
 */
int4 Commands::command_0x43( CmdArgs &args )
{
    return NoError;
}

//---------------------------------------------------------------------------------
/*
 * ctr_get_bit_weight(unsigned long axis, unsigned long unit, double * weight)
 * Code : 0x44
 */
int4 Commands::command_0x44( CmdArgs &args )
{
	uint4 axis = args.ui4[0];
	uint4 unit = args.ui4[1];

	if( axis >= MaxNumOfAxes ) {
		args.rl8[1] = 0.0;
		return CommandInvalidArgument;
	}

	switch( unit ) {
	    case 0:  args.rl8[1] = m_opticalSys.getBitPerMilli(axis); break;
	    case 1:  args.rl8[1] = m_opticalSys.getPulsePerBit(axis); break;
	    case 2:  args.rl8[1] = m_opticalSys.getPulsePerRad(axis); break;
	    default: args.rl8[1] = 0.0;                               break;
	}

	return NoError;
}

//---------------------------------------------------------------------------------
/*
 * ctr_focus_shift(long shift)
 * Code : 0x45
 */
int4 Commands::command_0x45( CmdArgs &args )
{
	CHECK_LIST_RUNNING

	m_mtnLinear.setOffset(Z, args.si4[0]);

	return m_mtnLinear.jumpRel(0, 0, 0, 0.0)->run(true);
}

//---------------------------------------------------------------------------------
/*
 * ctr_set_vsc_param(unsigned long enabled, unsigned long wait, unsigned long pre, unsigned long post)
 * Code : 0x46
 */
int4 Commands::command_0x46( CmdArgs &args )
{
	m_skyWriting->setParameter(args.ui4[0], args.ui4[1], args.ui4[2], args.ui4[3]);

	return NoError;
}

//---------------------------------------------------------------------------------
/*
 * ctr_set_vpc_param(unsigned long enabled, double mfreq, unsigned long amb, double vamp)
 * Code : 0x47
 */
int4 Commands::command_0x47( CmdArgs &args )
{

    return NoError;
}


//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
/*
 * ctr_download_correction_file
 */
int4 Commands::command_0x50( CmdArgs &args )
{

    return NoError;
}
//---------------------------------------------------------------------------------
/*
 * ctr_get_error_info
 */
int4 Commands::command_0x51( CmdArgs &args )
{
	args.ui4[0] = gp_plReg->getLatchedMasterError();
	for( uint4 i=0; i<MaxNumOfAxes; ++i ) {
		args.ui4[i+1] = gp_plReg->getLatchedDriverError(i);
		args.ui4[i+4] = gp_plReg->getDriverAlarmCode2(i);
	}

    return gp_plReg->getPlStatus();
}
//---------------------------------------------------------------------------------
/*
 * ctr_get_system_time
 */
int4 Commands::command_0x52( CmdArgs &args )
{
    return NoError;
}
//---------------------------------------------------------------------------------
/*
 * ctr_set_goto_speed()
 */
int4 Commands::command_0x53( CmdArgs &args )
{
	return m_scanner.setGotoSpeed(args.rl8[0])->run(true);
}
//---------------------------------------------------------------------------------
/*
 * ctr_get_measurement_signal
 */
int4 Commands::command_0x54( CmdArgs &args )
{
	ShmCommand cmd;
	cmd.code = ShmCmdCodeGetGatSignal;
	cmd.args.ui4[0] = args.ui4[0];

	gp_sharedMemory->runCommand(cmd);

	args.ui4[1] = cmd.args.ui4[1];

    return NoError;
}
//---------------------------------------------------------------------------------
/*
 * ctr_get_measurement_config
 */
int4 Commands::command_0x55( CmdArgs &args )
{
	ShmCommand cmd;
	cmd.code = ShmCmdCodeGetGatSignal;

	gp_sharedMemory->runCommand(cmd);

	args.ui4[0] = cmd.args.ui4[0];
	args.ui4[1] = cmd.args.ui4[1];

    return NoError;
}
//---------------------------------------------------------------------------------
/*
 * ctr_get_vsc_param
 */
int4 Commands::command_0x56( CmdArgs &args )
{
	m_skyWriting->getParameter(args.ui4[0], args.ui4[1], args.ui4[2], args.ui4[3]);

    return NoError;
}
//---------------------------------------------------------------------------------
/*
 * ctr_get_master_param(unsigned long id, double *value)
 */
int4 Commands::command_0x57( CmdArgs &args )
{
    return NoError;
}
//---------------------------------------------------------------------------------
/*
 * ctr_get_values(unsigned long *id, unsigned long *values)
 */
int4 Commands::command_0x58( CmdArgs &args )
{
	enum GetValueItems: uint4
	{
		BoardStatus1            = 0,
		BoardStatus2            = 1,
		CommandPosition_X_Pulse = 2,
		CommandPosition_Y_Pulse = 4,
		CommandPosition_Z_Pulse = 6,
		ScannerDriverStatus1    = 8,
		ScannerDriverStatus2    = 10,
		ScannerDriverStatus3    = 12,
		Dout4OutputValue        = 14,
		Dout16OutputValue       = 15,
		Dout12OutputValue       = 16,
		Da1OutputValue          = 17,
		Da2OutputValue          = 18,
		DIn4InputValue          = 19,
		DIn16InputValue         = 20,
	};

	for( int i=0; i<8; ++i ) {
		switch( args.ui4[i] ) {
		    case BoardStatus1:
	    		args.ui4[i] = gp_plReg->getRawValue(0, 0x40);
		    	break;
		    case BoardStatus2:
	    		args.ui4[i] = 0;
		    	break;
		    case CommandPosition_X_Pulse:
	    		args.ui4[i] = gp_plReg->getCmdPosPulse(X);
		    	break;
		    case CommandPosition_Y_Pulse:
	    		args.ui4[i] = gp_plReg->getCmdPosPulse(Y);
		    	break;
		    case CommandPosition_Z_Pulse:
	    		args.ui4[i] = gp_plReg->getCmdPosPulse(Z);
		    	break;
		    case ScannerDriverStatus1:
	    		args.ui4[i] = gp_plReg->getDriverStatus(X);
		    	break;
		    case ScannerDriverStatus2:
	    		args.ui4[i] = gp_plReg->getDriverStatus(Y);
		    	break;
		    case ScannerDriverStatus3:
	    		args.ui4[i] = gp_plReg->getDriverStatus(Z);
		    	break;
		    case Dout4OutputValue:
	    		args.ui4[i] = gp_plReg->getLaserIOValue(LaserPortDout4);
		    	break;
		    case Dout16OutputValue:
	    		args.ui4[i] = gp_plReg->getLaserIOValue(LaserPortDout16);
		    	break;
		    case Dout12OutputValue:
	    		args.ui4[i] = gp_plReg->getLaserIOValue(LaserPortDout12);
		    	break;
		    case Da1OutputValue:
	    		args.ui4[i] = gp_plReg->getLaserIOValue(LaserPortDA1);
		    	break;
		    case Da2OutputValue:
	    		args.ui4[i] = gp_plReg->getLaserIOValue(LaserPortDA2);
		    	break;
		    default:
	    		args.ui4[i] = 0;
		    	break;
		}
	}

    return NoError;
}
//---------------------------------------------------------------------------------
/*
 * ctr_get_fpga_reg(unsigned int baseAddr, unsigned int offset, unsigned int *value)
 */
int4 Commands::command_0x59( CmdArgs &args )
{
	args.ui4[2] = *((uint4*)(args.ui4[0] + args.ui4[1]));
    return NoError;
}
//---------------------------------------------------------------------------------
/*
 * ctr_get_fpga_block
 */
int4 Commands::command_0x5a( CmdArgs &args )
{
    return NoError;
}
//---------------------------------------------------------------------------------
/*
 * ctr_get_fpga_info
 */
int4 Commands::command_0x5b( CmdArgs &args )
{
    return NoError;
}
//---------------------------------------------------------------------------------
/*
 * int ctr_get_list_info(unsigned long list_no, unsigned long *info)
 */
int4 Commands::command_0x5c( CmdArgs &args )
{
	ListBoxCh ch = (ListBoxCh)(args.ui4[0] - 1);

	args.ui4[0] = gp_listManager->getStatusVal(ch);
	args.ui4[1] = gp_listManager->getWriteIndex(ch);
	args.ui4[2] = gp_listManager->getReadIndex(ch);
	args.ui4[3] = gp_plReg->getPlStatus();
	args.ui4[4] = gp_plReg->getPsWriteIndex();
	args.ui4[5] = gp_plReg->getPlReadIndex();
	args.ui4[6] = gp_plReg->getPlMoveIndex();

	return NoError;
}
//---------------------------------------------------------------------------------
/*
 * int ctr_set_optical_param(float D, float d, unsigned int latticeInterval, unsigned int geocompDisabled)
 */
int4 Commands::command_0x5d( CmdArgs &args )
{
	return m_opticalSys.setParameter(args.rl4[0], args.rl4[1], args.rl4[2], args.rl4[3])->run(true);
}
//---------------------------------------------------------------------------------
/*
 * int ctr_get_optical_param(float *D, float *d, unsigned int *latticeInterval, unsigned int *geocompDisabled)
 */
int4 Commands::command_0x5e( CmdArgs &args )
{
	args.rl4[0] = m_opticalSys.getGeoCompParam1();
	args.rl4[1] = m_opticalSys.getGeoCompParam2();
	args.ui4[2] = (uint4)m_opticalSys.getLatticeInterval();
	args.ui4[3] = (uint4)m_opticalSys.isGeoCompDisabled();

	return NoError;
}
//---------------------------------------------------------------------------------
/*
 * int ctr_set_bit_weight(unsigned int axis, unsigned int unit, double weight)
 */
int4 Commands::command_0x5f( CmdArgs &args )
{
	uint4 axis = args.ui4[0];
	uint4 unit = args.ui4[1];

	if( axis >= MaxNumOfAxes ) {
		return CommandInvalidArgument;
	}

	switch( unit ) {
	    case 0:
	    	return m_opticalSys.setBitPerMilli(axis, (real4)args.rl8[1])->run(true);
	    case 1:
	    	return m_opticalSys.setPulsePerBit(axis, (real4)args.rl8[1])->run(true);
	    case 2:
	    	return m_opticalSys.setPulsePerRad(axis, (real4)args.rl8[1])->run(true);
	    default:
	    	break;
	}

	return NoError;
}
//---------------------------------------------------------------------------------
/*
 * int ctr_change_status(unsigned int chBit, unsigned int status)
 */
int4 Commands::command_0x60( CmdArgs &args )
{
	return NoError;
}
//---------------------------------------------------------------------------------
/*
 * int ctr_enable_measurement()
 */
int4 Commands::command_0x61( CmdArgs &args )
{
    return m_gathering.enable();
}
//---------------------------------------------------------------------------------
/*
 * int ctr_disable_measurement()
 */
int4 Commands::command_0x62( CmdArgs &args )
{
    return m_gathering.disable();
}
//---------------------------------------------------------------------------------
/*
 * int ctr_set_vsc_mode(unsigned int mode)
 */
int4 Commands::command_0x63( CmdArgs &args )
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
/*
 * int ctr_set_vsc_limit(double limit)
 */
int4 Commands::command_0x64( CmdArgs &args )
{
	m_skyWritingMode3.setLimit(args.rl8[0]);
    return NoError;
}
//---------------------------------------------------------------------------------
/*
 */
int4 Commands::command_0x65( CmdArgs &args )
{
    return NoError;
}
//---------------------------------------------------------------------------------
/*
 */
int4 Commands::command_0x66( CmdArgs &args )
{
    return NoError;
}
//---------------------------------------------------------------------------------
/*
 */
int4 Commands::command_0x67( CmdArgs &args )
{
    return NoError;
}
//---------------------------------------------------------------------------------
/*
 */
int4 Commands::command_0x68( CmdArgs &args )
{
    return NoError;
}
//---------------------------------------------------------------------------------
/*
 */
int4 Commands::command_0x69( CmdArgs &args )
{
    return NoError;
}
//---------------------------------------------------------------------------------
/*
 */
int4 Commands::command_0x6a( CmdArgs &args )
{
    return NoError;
}
//---------------------------------------------------------------------------------
/*
 */
int4 Commands::command_0x6b( CmdArgs &args )
{
    return NoError;
}
//---------------------------------------------------------------------------------
/*
 */
int4 Commands::command_0x6c( CmdArgs &args )
{
    return NoError;
}
//---------------------------------------------------------------------------------
/*
 */
int4 Commands::command_0x6d( CmdArgs &args )
{
    return NoError;
}
//---------------------------------------------------------------------------------
/*
 */
int4 Commands::command_0x6e( CmdArgs &args )
{
    return NoError;
}
//---------------------------------------------------------------------------------
/*
 */
int4 Commands::command_0x6f( CmdArgs &args )
{
    return NoError;
}
//---------------------------------------------------------------------------------
/*
 */
int4 Commands::command_0x70( CmdArgs &args )
{
    return NoError;
}
//---------------------------------------------------------------------------------
/*
 */
int4 Commands::command_0x71( CmdArgs &args )
{
    return NoError;
}
//---------------------------------------------------------------------------------
/*
 */
int4 Commands::command_0x72( CmdArgs &args )
{
    return NoError;
}
//---------------------------------------------------------------------------------
/*
 * ctr_set_simulation_mode(unsigned int enabled)
 */
int4 Commands::command_0x73( CmdArgs &args )
{
	gp_log->debug("ctr_set_simulation_mode(%u)", args.ui4[0]);
	if( args.ui4[0] != 0 ) {
		gp_plReg->setSimModeRequest();
		gp_plReg->setAckRequest();
	}
    return NoError;
}
//---------------------------------------------------------------------------------
/*
 * ctr_clear_error()
 */
int4 Commands::command_0x74( CmdArgs &args )
{
	gp_plReg->setAckRequest();
    return NoError;
}
//---------------------------------------------------------------------------------
/*
 */
int4 Commands::command_0x75( CmdArgs &args )
{
    return NoError;
}
//---------------------------------------------------------------------------------
/*
 */
int4 Commands::command_0x76( CmdArgs &args )
{
    return NoError;
}
//---------------------------------------------------------------------------------
/*
 */
int4 Commands::command_0x77( CmdArgs &args )
{
    return NoError;
}
//---------------------------------------------------------------------------------
/*
 */
int4 Commands::command_0x78( CmdArgs &args )
{
    return NoError;
}
//---------------------------------------------------------------------------------
/*
 */
int4 Commands::command_0x79( CmdArgs &args )
{
    return NoError;
}
//---------------------------------------------------------------------------------
/*
 */
int4 Commands::command_0x7a( CmdArgs &args )
{
    return NoError;
}
//---------------------------------------------------------------------------------
/*
 */
int4 Commands::command_0x7b( CmdArgs &args )
{
    return NoError;
}
//---------------------------------------------------------------------------------
/*
 */
int4 Commands::command_0x7c( CmdArgs &args )
{
    return NoError;
}
//---------------------------------------------------------------------------------
/*
 */
int4 Commands::command_0x7d( CmdArgs &args )
{
    return NoError;
}
//---------------------------------------------------------------------------------
/*
 */
int4 Commands::command_0x7e( CmdArgs &args )
{
    return NoError;
}
//---------------------------------------------------------------------------------
/*
 */
int4 Commands::command_0x7f( CmdArgs &args )
{
    return NoError;
}
//---------------------------------------------------------------------------------
