/*
 * This Source Code Form is subject to the terms
 * of the *GNU GENERAL PUBLIC LICENSE Version 3*.
 *
 * Authors:
 *  - 2018, Annika Ofenloch (DLR RY-AVS)
 */

#ifndef __ROUTER_H__
#define __ROUTER_H__

#include <fstream>
#include <queue>
#include <string>
#include <stdint.h>
#include <bitset>
#include <iostream>

#include "utils/flit_utils.h"

class Router {
public:
	Router() {
	}

	enum Request
		: uint8_t {
			idle = 0, local = 1, north = 2, east = 3, south = 4, west = 5
	};

	bool arbitrateWithRoundRobinPrioritization();

	bool pushToLocalFIFO(uint32_t flit);
	bool pushToWestFIFO(uint32_t flit);
	bool pushToEastFIFO(uint32_t flit);
	bool pushToSouthFIFO(uint32_t flit);
	bool pushToNorthFIFO(uint32_t flit);

	void increaseCreditCntNorth();
	void increaseCreditCntWest();
	void increaseCreditCntEast();
	void increaseCreditCntSouth();

	// Getter & Setter
	void setNocSize(uint16_t size) {
		mNocSize = size;
	}

	void setAddress(uint16_t address) {
		mCurrentAddr = address;
	}

	void setRoutingBits(std::bitset<16> rountingBits) {
		mRoutingBits = rountingBits;
	}

	void setConnectivityBits(std::bitset<16> connectivityBits) {
		mConnectivityBits = connectivityBits;
	}

	uint32_t getNextFlit() {
		return mNextFlit;
	}

	std::string getChosenOutputPort() {
		return mChosenOutputPort;
	}

	std::string getCreditCntSignal() {
		return mCreditCntSignal;
	}

private:
	uint64_t mCycles = 0; // TODO: Forward Flit after 3 Cycles
	uint16_t mNocSize = 0;
	uint16_t mCurrentAddr = 0;
	std::bitset<16> mConnectivityBits = 0;
	std::bitset<16> mRoutingBits = 0;

	// Credit based flow
	uint16_t mCredit_Cnt_W = 3;
	uint16_t mCredit_Cnt_N = 3;
	uint16_t mCredit_Cnt_S = 3;
	uint16_t mCredit_Cnt_E = 3;

	std::queue<uint32_t> mFlits_RX_L; // L FIFO
	std::queue<uint32_t> mFlits_RX_N; // N FIFO
	std::queue<uint32_t> mFlits_RX_E; // E FIFO
	std::queue<uint32_t> mFlits_RX_S; // S FIFO
	std::queue<uint32_t> mFlits_RX_W; // W FIFO

	// Generated requests from LDBRs
	Request mReq_N_LBDR = idle;
	Request mReq_E_LBDR = idle;
	Request mReq_W_LBDR = idle;
	Request mReq_S_LBDR = idle;
	Request mReq_L_LBDR = idle;

	// Generated requests from LDBRs
	bool north_grant = false;
	bool east_grant = false;
	bool west_grant = false;
	bool south_grant = false;
	bool local_grant = false;

	uint32_t mNextFlit = 0;
	std::string mChosenOutputPort;
	std::string mCreditCntSignal;

	bool validGrantSignal(Request& request, std::queue<uint32_t>& flitFIFO,
			std::string creditString);

	// returns the request (Local, North, East, South or West)
	void generateRequest(uint32_t flit, Request& request);
};

#endif /* __ROUTER_H__ */
