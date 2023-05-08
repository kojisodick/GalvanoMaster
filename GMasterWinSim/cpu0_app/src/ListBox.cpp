/*
 * ListBox.cpp
 *
 *  Created on: 2018/11/15
 *      Author: imao_yusuke
 */

#include <ListBox.h>

#include <Logger/Logger.h>
extern Logger *gp_log;

static void checkMoveLength(const CommandData &cmd)
{
	static int4 x = 0, y = 0;
	int4 sX = x, sY = y;
	real8 length = 0.0;

	switch( cmd.code ) {
	case CmdCodeListJumpAbs:
		x = cmd.args.si4[0];
		y = cmd.args.si4[1];
		break;
	case CmdCodeListJumpRel:
		x += cmd.args.si4[0];
		y += cmd.args.si4[1];
		break;
	case CmdCodeListMarkAbs:
		x = cmd.args.si4[0];
		y = cmd.args.si4[1];
		length = ::distance2D(sX, sY, x, y);
		break;
	case CmdCodeListMarkRel:
		x += cmd.args.si4[0];
		y += cmd.args.si4[1];
		length = ::distance2D(sX, sY, x, y);
		break;
	default:
		break;
	}

	if( length > (3488 * 50) ) {
		gp_log->error("[ListBox::write] long_mark:(%d, %d) -> (%d, %d)", sX, sY, x, y);
	}
}



//--------------------------------------------------------------
void ListBox::clear()
{
	memset(m_command, 0, sizeof(m_command));
	m_writeIndex   = 0;
	m_readIndex    = 0;
	m_status       = StsEmpty;
}

//--------------------------------------------------------------
int4 ListBox::startWriting(uint4 index)
{
	if( m_status == StsRunning ){
		gp_log->error("[ListBox::startWriting] ListBox is already running.");
		return ListIsRunning;
	}

	if( index >= MAX_NUM_OF_CMD_IN_LSTBOX ) {
		return InvalidWriteIndex;
	}

	/* GAL-96 */
	this->clear();

	m_writeIndex = index;
	m_status     = StsLoading;

	gp_log->debug("[ListBox::startWriting] set (wIndex=%u).", m_writeIndex);

	return NoError;
}

//--------------------------------------------------------------
int4 ListBox::startReading(uint4 index)
{
	if( m_status != StsReady ) {
		gp_log->error("[Listbox::startReading] invalid state(%x).",  m_status);
		return ListIsNotReady;
	}

	if( index >= MAX_NUM_OF_CMD_IN_LSTBOX ) {
		gp_log->error("[Listbox::startReading] invalid index(%d).",  index);
		return InvalidReadIndex; //invalid index;
	}

	m_readIndex = index;
	gp_log->debug("[ListBox::startReading] set (rIndex=%u).", m_readIndex);
	
	m_status    = StsRunning;

	return NoError;
}

//--------------------------------------------------------------
int4 ListBox::push(const CommandData &cmd)
{
	if( m_status != StsLoading ) {
		gp_log->error("[Listbox::write] Invalid state(%x)->cmdCode(0x%x).", m_status, cmd.code);
		return ListIsNotLoading;
	}

	if( m_writeIndex >= MAX_NUM_OF_CMD_IN_LSTBOX ) {
		gp_log->error("[Listbox::write] WriteIndex is invalid(%d).", m_writeIndex);
		return BufferOverflowed;
	}


	/* GAL-96 */
	for( int i=0; i<3; ++i ) {
		memcpy(&m_command[m_writeIndex], &cmd, SIZE_OF_CMDDATA);
		if( m_command[m_writeIndex].code == cmd.code ) {
			break;
		}
		gp_log->error("[ListBox::write] Retry %d :wIndex=%u", i+1, m_writeIndex);
		gp_log->error("[ListBox::write] cmdCode=%u, args[0]=%x, args[1]=%x, args[2]=%x"
				,cmd.code, cmd.args.ui4[0], cmd.args.ui4[1], cmd.args.ui4[2]);
	}

	//gp_log->info("[%u] cmd=0x%x", m_writeIndex, cmd.code);

	m_writeIndex++;

	if( cmd.code == CmdCodeListSetEndOfList ) {
		m_status = StsReady;
		gp_log->debug("[ListBox::write] write (lst_set_end_of_list). (wIndex=%u)", m_writeIndex);
	}

	/* GAL-96 */
#ifdef DEBUG_GAL96
    checkMoveLength(cmd);
#endif

	return NoError;
}
//--------------------------------------------------------------
int4 ListBox::pop(CommandData &data)
{
	if( m_status != StsRunning ) {
		gp_log->debug("[Listbox::read] ListBox is not running.(sts=%x)",  m_status);
		return ListIsNotRunning;
	}

	if( m_readIndex >= m_writeIndex ){
		gp_log->info("[Listbox::read] ListBox is empty. (index:%d)",  m_readIndex);
		this->clear();
		return InvalidReadIndex;
	}

	memcpy(&data, &m_command[m_readIndex], SIZE_OF_CMDDATA);

	m_readIndex++;

	return NoError;
}
