#include <iostream>

#include "controller.hh"
#include "timestamp.hh"

using namespace std;


#define CWND_INIT 50
#define W_MAX 100
#define SLOW_GROWTH 1 
#define CGROWTH_RATE 1
#define T_TIMEOUT 1000

/* Default constructor */
Controller::Controller( const bool debug )
  : debug_( debug ), cwnd( CWND_INIT ), sthresh( W_MAX / 2)
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
  if (cwnd < sthresh)
  {
    /* Slow Start...*/
    if (timestamp_ack_received - send_timestamp_acked > T_TIMEOUT)
    {
      sthresh = sthresh / 2;
      cwnd = cwnd / 2;
    }
    else
        cwnd += SLOW_GROWTH;   
  }
  else 
  {
    /* Congestion Avoidance... */
    cwnd += (CGROWTH_RATE)* 1/cwnd;
  }

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