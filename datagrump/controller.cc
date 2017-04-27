#include <iostream>

#include "controller.hh"
#include "timestamp.hh"

using namespace std;


#define CWND_INIT 50
#define CWND_RSET 1
#define T_TIMEOUT 500
// #define THRESH 100
// #define INCR 2
// #define DECR 12



/* Default constructor */
Controller::Controller( const bool debug )
  : debug_( debug ), cwnd( CWND_INIT ), toggle(0)
{}

/* Get current window size, in datagrams */
unsigned int Controller::window_size( void )
{
  /* Default: fixed window size of 100 outstanding datagrams */
  // unsigned int the_window_size = 50;
    // cerr << "WINDOW: "<< cwnd << endl;
  if ( debug_ ) {
    cerr << "At time " << timestamp_ms()
	 << " window size is " << cwnd << endl;
  }

  return cwnd;
}

/* A datagram was sent */
void Controller::datagram_was_sent( const uint64_t sequence_number,
				    /* of the sent datagram */
				    const uint64_t send_timestamp )
                                    /* in milliseconds */
{
  /* Default: take no action */
    // if(toggle == 10)
    // {
    //   cerr << "WINDOW: " << cwnd << endl;
    //   toggle = 0;
    // }
    // else
    //   toggle++;

  if ( debug_ ) {
    cerr << "At time " << send_timestamp
	 << " sent datagram " << sequence_number << endl;
  }
}

/* An ack was received */
void Controller::ack_received( const uint64_t sequence_number_acked,
			       /* what sequence number was acknowledged */
			       const uint64_t send_timestamp_acked,
			       /* when the acknowledged datagram was sent (sender's clock) */
			       const uint64_t recv_timestamp_acked,
			       /* when the acknowledged datagram was received (receiver's clock)*/
			       const uint64_t timestamp_ack_received )
                               /* when the ack was received (by sender) */
{
  /* Default: take no action */
  // bool timed_out = (timestamp_ack_received - send_timestamp_acked) > timeout_ms(); 

  // if (timed_out)
  // {
  //   // cerr << "DROPPED PACKET" << endl;
  //   cwnd = CWND_RSET;
  // }
  // else
  //   cwnd = cwnd + 1/cwnd;

  // bool over_threshold = (timestamp_ack_received - send_timestamp_acked) > THRESH;
  // if (over_threshold)
  // {
  //   cwnd -= DECR;
  //   if (cwnd < 1 || cwnd > 4200000000)
  //     cwnd = 1;
  // }
  // else
  //   cwnd += INCR;
  #define INCR_N1 6
  #define THSH_N1 20

  #define INCR_0 2
  #define THSH_0 50
  
  #define INCR_1 1
  #define THSH_1 100
  
  #define INCR_2 (-1)
  #define THSH_2 125
  
  #define INCR_3 (-2)
  #define THSH_3 150
  
  #define INCR_4 (-10)
  #define THSH_4 300

  #define INCR_5 (-20)

  unsigned int RTT = timestamp_ack_received - send_timestamp_acked;

  // Only increment if there's low queue delay

  if (RTT <= THSH_N1)
    cwnd += INCR_N1;

  else if (RTT <= THSH_0)
    cwnd += INCR_0;

  else if (RTT <= THSH_1)
    cwnd += INCR_1;

  else if (RTT <= THSH_2)
    cwnd += INCR_2;

  else if (RTT <= THSH_3)
    cwnd += INCR_3;

  else if (RTT <= THSH_4)
    cwnd /= 2;

  else
    cwnd /= 3;
    // cwnd = CWND_RSET;


  // Catch all, prevents empty/negative cwnds
  if (cwnd < 1 || cwnd > 4200000000)
    cwnd = 1;


  if ( debug_ ) {
    cerr << "At time " << timestamp_ack_received
	 << " received ack for datagram " << sequence_number_acked
	 << " (send @ time " << send_timestamp_acked
	 << ", received @ time " << recv_timestamp_acked << " by receiver's clock)"
	 << endl;
  }
}

/* How long to wait (in milliseconds) if there are no acks
   before sending one more datagram */
unsigned int Controller::timeout_ms( void )
{
  return T_TIMEOUT; /* timeout of one second */
}