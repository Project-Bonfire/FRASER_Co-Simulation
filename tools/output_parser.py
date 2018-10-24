#!/usr/bin/python3

import sys
import argparse

COLOR_RED = "\033[1;31m"
COLOR_GREEN = "\033[1;32m"
COLOR_YELLOW = "\033[1;33m"
COLOR_BLUE = "\033[1;34m"
COLOR_BOLD = "\033[1m"
COLOR_DEFAULT = "\033[0m"

sentPackets = []
recvdPackets = []
faultyPackets = []
missingPacketsList = []
errors = []

def parsePacket(line):
    if '[S]' in line:
        sentPackets.append(line)
    
    elif '[R]' in line:
        recvdPackets.append(line)

        if '[FAULTY]' in line:
            faultyPackets.append(line)
    else:
        print(COLOR_RED, 'Parse error, badly formatted line: ', COLOR_DEFAULT, '\n\t', line)


def doStatistics():
    print(COLOR_BOLD + 'Statistics:' + COLOR_DEFAULT)

    print('Total packets sent: ', len(sentPackets))
    print('Total packets received: ', len(recvdPackets))

    missingPackets = len(sentPackets) - len(recvdPackets)
    print('Packets missing: ', COLOR_GREEN if missingPackets == 0 else COLOR_RED, 
            missingPackets, COLOR_DEFAULT)

    if missingPackets > 0:
        for sentPacketString in sentPackets:

            sentSrc = sentPacketString.split('- ')[1].split(', ')[0].split(' ')[1]
            sentId = sentPacketString.split('- ')[1].split(', ')[2].split(' ')[1]
            
            packetFound = False

            for recvPacketString in recvdPackets:
                recvdSrc = recvPacketString.split('- ')[1].split(', ')[0].split(' ')[1]
                recvdId = recvPacketString.split('- ')[1].split(', ')[2].split(' ')[1]
                
                if recvdSrc == sentSrc and recvdId == sentId:
                    packetFound = True
                    break
            
            if not packetFound:
                missingPacketsList.append(sentPacketString)

        print(COLOR_BOLD + "The following packets were lost:\n" + COLOR_DEFAULT)
        for missingPacket in missingPacketsList:
            print('    ' + missingPacket.strip())

    print()

    print('Number of faulty packets received: ', 
            COLOR_GREEN if len(faultyPackets) == 0 else COLOR_RED, 
            len(faultyPackets), COLOR_DEFAULT)

    if len(faultyPackets) > 0:
        print(COLOR_RED, 'The faulty packets were the following:', COLOR_DEFAULT)

        for packet in faultyPackets:
            print(packet)

    print()

    print('Number of errors found during the run: ', 
            COLOR_GREEN if len(errors) == 0 else COLOR_RED, 
            len(errors), COLOR_DEFAULT)

    if len(errors) > 0:
        print(COLOR_RED, 'The errors were the following:', COLOR_DEFAULT)

        for error in errors:
            print(error)


def main():
    # if len(sys.argv) != 2 or '-h' in sys.argv or '--help' in sys.argv:
    #     printUsage()
    #     sys.exit(0)

    parser = argparse.ArgumentParser(description='Traffic generation output parser.')
    parser.add_argument('--logfile', '-f', type=str, required=True,
                        help='Log file to parse')
    parser.add_argument('--listSent', '-ls', action='store_true', required=False,
                        help='List all sent packets')
    parser.add_argument('--listRecv', '-lr',  action='store_true', required=False,
                        help='List all received packet')

    args = parser.parse_args()
 
    with open(args.logfile, 'r') as logFile:

        for line in logFile:
            if '[PACKET]' in line:
                parsePacket(line)
            
            elif '[ERROR]' in line:
                errors.append(line)

    doStatistics()

    if args.listSent:
        print()
        print(COLOR_BOLD + 'All sent packets:' + COLOR_DEFAULT)
        for packet in sentPackets:
            print(packet.strip())

    if args.listRecv:
        print()
        print(COLOR_BOLD + 'All received packets:' + COLOR_DEFAULT)
        for packet in recvdPackets:
            print(packet.strip())

if __name__ == '__main__':
    if sys.version_info[0] < 3:
        raise Exception('The script only works in Python 3!')

    main()