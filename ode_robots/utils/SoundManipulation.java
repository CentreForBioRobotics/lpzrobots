/*
 Converts sensor values into sound.
*/

import javax.sound.midi.*;
//import javax.sound.sampled.*;
import java.io.*;

public class SoundManipulation extends Thread {

 private int mode;
 private float param;
 private int numSensors;
// private SourceDataLine sourceLine;
 private InputStream is;
 private Receiver synthRcvr;

 public SoundManipulation(int mode, float param, InputStream is) {
  this.mode=mode;
  this.param=param;
  this.is=is;
/*
  AudioFormat format=new AudioFormat(48000.0f,16,1,true,false);
  DataLine.Info sourceInfo=new DataLine.Info(SourceDataLine.class,format);
  if(!AudioSystem.isLineSupported(sourceInfo)) {
   System.out.println("SoundOutput: Line not supported.");
   System.exit(0);
  }
  try {
   sourceLine=(SourceDataLine)AudioSystem.getLine(sourceInfo);
   sourceLine.open(format);
   sourceLine.start();
  } catch(LineUnavailableException lue) {
   System.out.println(lue.getMessage());
   System.exit(0);
  }
*/

  Sequencer seq;
  Transmitter seqTrans;
  Synthesizer synth;
  try {
   seq=MidiSystem.getSequencer();
   seqTrans=seq.getTransmitter();
   synth=MidiSystem.getSynthesizer();
   synth.open();
   synthRcvr=synth.getReceiver();
   seqTrans.setReceiver(synthRcvr);
  } catch(MidiUnavailableException mue) {
   System.out.println(mue.getMessage());
   System.exit(0);
  }

 }

 public void run() {
  byte[] data=new byte[128];
  String input="";

  while(true) {
   int next=0;
   try {
    next=is.read();
   } catch(IOException ioe) {
    System.out.println(ioe.getMessage());
    System.exit(0);
   }
   if(next==-1) break;
   input+=(char)next;
   if((char)next=='\n') {
    if(input.startsWith("#N neuron x[")) {
     numSensors=input.charAt(12)-47;
    } else if(!input.startsWith("#")) {
     String[] values=input.trim().substring(0,numSensors*10).split(" ");
     float sensorMax=0.0f;
     for(int i=0; i<numSensors; i++) {

      switch(mode) {
       case 1: // discrete
        float sensVal=Math.abs(new Float(values[i]).floatValue());
        if(sensVal>param) {
         try {
          ShortMessage sm=new ShortMessage();
          sm.setMessage(ShortMessage.NOTE_ON, Math.min(i,15), 50+20*i/numSensors, 90);
          synthRcvr.send(sm, -1);
         } catch(InvalidMidiDataException imde) {
          System.out.println(imde.getMessage());
          System.exit(0);
         }
        }
        break;
       case 2: // amplitude
        sensorMax=Math.max(sensorMax,Math.abs(new Float(values[i]).floatValue()));
        if(i==numSensors-1) {
         try {
          ShortMessage sm=new ShortMessage();
          sm.setMessage(ShortMessage.NOTE_ON, 0, 50, (int)(param*Math.pow(sensorMax,3.0)*20.0f));
          synthRcvr.send(sm, -1);
         } catch(InvalidMidiDataException imde) {
          System.out.println(imde.getMessage());
          System.exit(0);
         }
         sensorMax=0.0f;
        }
        break;
       case 3: // frequency
        sensorMax=Math.max(sensorMax,Math.abs(new Float(values[i]).floatValue()));
        if(i==numSensors-1) {
         try {
          ShortMessage sm=new ShortMessage();
          sm.setMessage(ShortMessage.NOTE_ON, 0, (int)(param*Math.pow(sensorMax,3.0)*15.0f), 50);
          synthRcvr.send(sm, -1);
         } catch(InvalidMidiDataException imde) {
          System.out.println(imde.getMessage());
          System.exit(0);
         }
         sensorMax=0.0f;
        }
        break;

      }

     }
    }
    input="";
   }
  }

 }
}