///////////////////////////////////////////////////////////
//   UART Task v1.0                                      //
//   ==============                                      //
//   Copyright© 2017 by Felix Knobl, FH Technikum Wien   //
//   Partial codeparts by Martin Horauer                 //
///////////////////////////////////////////////////////////

Based on the XMC4500 Interface Example realized a small communication protocol that follows a bi-directional frame-based protocol pattern relying entirely on ASCII characters.

Every frame starts with a "start-of-frame" (SoF) character (#), followed by a "command" (CMD) character, a separator (SEP), a variable number of "DATA" characters, and an "end-of-frame" (EoF) terminating character (\n).

Every command shall be acknowledged from the receiving side with an ACK in case of success and NACK message in case of an error.
