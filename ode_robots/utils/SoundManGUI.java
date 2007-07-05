import javax.swing.*;
import javax.swing.event.*;
import java.awt.event.*;
import java.text.*;

public class SoundManGUI extends JFrame {
 private float param;
 private int mode;
 private int numSensors;
 private int curInstrument;
 private boolean isPlaybackActive;
 private int note;
 private int toneLength;
 private JComboBox instruCB;
 private JLabel l4;
 private JSlider noteSlide;
 private JSlider toneLSlide;
 private JSlider[] sensorSlide;
 private JCheckBox[] sensorCheck;

 public SoundManGUI(float p, int m) {
  param=p;
  mode=m;
  toneLength=150;
  if(mode>=3) noteSlide.setEnabled(false);
  isPlaybackActive=true;
  getContentPane().setLayout(null);
  setTitle("SoundMan");
  setResizable(false);
  setSize(250,320);


  ////////// param //////////
  JLabel l1=new JLabel("Parameter:");
  l1.setBounds(10, 70, 80, 20);
  getContentPane().add(l1);

  DecimalFormat df=new DecimalFormat();
  df.setMaximumIntegerDigits(1);
  df.setMinimumIntegerDigits(1);
  df.setMinimumFractionDigits(1);
  df.setDecimalSeparatorAlwaysShown(true);
  final JFormattedTextField paramFTF=new JFormattedTextField(df);
  paramFTF.setText(NumberFormat.getInstance().format(param));
  paramFTF.setBounds(85, 70, 50, 20);
  getContentPane().add(paramFTF);
  paramFTF.addActionListener(
   new ActionListener() {
    public void actionPerformed(ActionEvent e) {
     float newParam=new Float(paramFTF.getText().replace(',','.')).floatValue();
     if(newParam<0.0f) {
      newParam=Math.abs(newParam);
      paramFTF.setText(NumberFormat.getInstance().format(newParam));
     }
     param=newParam;
    }
   }
  );

  final JButton paramB=new JButton("Accept");
  paramB.setBounds(150, 70, 75, 20);
  getContentPane().add(paramB);
  paramB.addActionListener(
   new ActionListener() {
    public void actionPerformed(ActionEvent e) {
     float newParam=new Float(paramFTF.getText().replace(',','.')).floatValue();
     if(newParam<0.0f) {
      newParam=Math.abs(newParam);
      paramFTF.setText(NumberFormat.getInstance().format(newParam));
     }
     param=newParam;
    }
   }
  );

  ////////// mode //////////
  JRadioButton discRB=new JRadioButton("Discrete");
  discRB.setBounds(5, 10, 75, 20);
  getContentPane().add(discRB);
  discRB.addActionListener(
   new ActionListener() {
    public void actionPerformed(ActionEvent e) {
     mode=1;
     noteSlide.setEnabled(true);
     paramFTF.setEnabled(true);
     paramB.setEnabled(true);
    }
   }
  );

  JRadioButton amplRB=new JRadioButton("Amplitude");
  amplRB.setBounds(80, 10, 80, 20);
  getContentPane().add(amplRB);
  amplRB.addActionListener(
   new ActionListener() {
    public void actionPerformed(ActionEvent e) {
     mode=2;
     noteSlide.setEnabled(true);
     paramFTF.setEnabled(true);
     paramB.setEnabled(true);
    }
   }
  );

  JRadioButton freqRB=new JRadioButton("Frequency");
  freqRB.setBounds(160, 10, 80, 20);
  getContentPane().add(freqRB);
  freqRB.addActionListener(
   new ActionListener() {
    public void actionPerformed(ActionEvent e) {
     mode=3;
     noteSlide.setEnabled(false);
     paramFTF.setEnabled(true);
     paramB.setEnabled(true);
    }
   }
  );

  JRadioButton masterRB=new JRadioButton("Master mode (all three combined)");
  masterRB.setBounds(5, 40, 255, 20);
  getContentPane().add(masterRB);
  masterRB.addActionListener(
   new ActionListener() {
    public void actionPerformed(ActionEvent e) {
     mode=4;
     noteSlide.setEnabled(false);
     paramFTF.setEnabled(false);
     paramB.setEnabled(false);
    }
   }
  );

  ButtonGroup modeBG=new ButtonGroup();
  modeBG.add(discRB);
  modeBG.add(amplRB);
  modeBG.add(freqRB);
  modeBG.add(masterRB);

  if(mode==1) discRB.setSelected(true);
  else if(mode==2) amplRB.setSelected(true);
  else if(mode==3) freqRB.setSelected(true);
  else masterRB.setSelected(true);

  ////////// note //////////
  JLabel l2=new JLabel("Note:");
  l2.setBounds(10, 95, 35, 20);
  getContentPane().add(l2);

  noteSlide=new JSlider(0, 100, 50);
  noteSlide.setBounds(45, 95, 190, 20);
  getContentPane().add(noteSlide);
  note=noteSlide.getValue();
  noteSlide.addChangeListener(
   new ChangeListener() {
    public void stateChanged(ChangeEvent e) {
     note=noteSlide.getValue();
    }
   }
  );

  ////////// tone length //////////
  JLabel l6=new JLabel("Tone length in ms:");
  l6.setBounds(10, 120, 120, 20);
  getContentPane().add(l6);

  toneLSlide=new JSlider(50, 1000, 150);
  toneLSlide.setBounds(125, 120, 115, 35);
  toneLSlide.setMajorTickSpacing(950);
  toneLSlide.setPaintLabels(true);
  getContentPane().add(toneLSlide);
  toneLSlide.addChangeListener(
   new ChangeListener() {
    public void stateChanged(ChangeEvent e) {
     toneLength=toneLSlide.getValue();
    }
   }
  );

  ////////// instruments //////////
  JLabel l3=new JLabel("Instrument:");
  l3.setBounds(10, 155, 80, 20);
  getContentPane().add(l3);

  instruCB=new JComboBox();
  instruCB.setBounds(90, 155, 140, 20);
  getContentPane().add(instruCB);

  ////////// sensors //////////
  l4=new JLabel("Number of sensors: -");
  l4.setBounds(10, 175, 200, 20);
  getContentPane().add(l4);

  ////////// playback //////////
  JLabel l5=new JLabel("Playback:");
  l5.setBounds(10, 260, 200, 20);
  getContentPane().add(l5);

  final JButton playbackB=new JButton("Mute");
  playbackB.setBounds(90, 260, 140, 20);
  getContentPane().add(playbackB);
  playbackB.addActionListener(
   new ActionListener() {
    public void actionPerformed(ActionEvent e) {
     isPlaybackActive=!isPlaybackActive;
     if(isPlaybackActive) playbackB.setText("Mute");
     else playbackB.setText("Unmute");
    }
   }
  );
 }

 public int getToneLength() {
  return toneLength;
 }

 public int getNote() {
  return note;
 }

 public void setNote(int newNote) {
  note=newNote;
  noteSlide.setValue(note);
 }

 public boolean isPlaybackActive() {
  return isPlaybackActive;
 }

 public void setNumSensors(int n) {
  if(numSensors==n || n==0) return;
  numSensors=n;
  l4.setText("Number of sensors: "+n);

  sensorSlide=new JSlider[n];
  for(int i=0; i<n; i++) {
   sensorSlide[i]=new JSlider(JSlider.VERTICAL,0, 100, 0);
   sensorSlide[i].setEnabled(false);
   sensorSlide[i].setBounds((i+1)*230/(n+1), 190, 10, 50);
   getContentPane().add(sensorSlide[i]);
   getContentPane().repaint();
  }

  sensorCheck=new JCheckBox[n];
  for(int i=0; i<n; i++) {
   sensorCheck[i]=new JCheckBox("",true);
   sensorCheck[i].setBounds((i+1)*230/(n+1)-5, 241, 16, 12);
   getContentPane().add(sensorCheck[i]);
   getContentPane().repaint();
  }
 }

 public boolean isSensorChecked(int i) {
  return sensorCheck[i].isSelected();
 }

 public void setSensorValue(int num, float val) {
  sensorSlide[num].setValue((int)(val*100.0));
 }

 public float getParam() {
  return param;
 }

 public int getMode() {
  return mode;
 }

 public boolean instrumentChanged() {
  if(curInstrument!=instruCB.getSelectedIndex()) {
   curInstrument=instruCB.getSelectedIndex();
   return true;
  }
  return false;
 }

 public int getInstrument() {
  return instruCB.getSelectedIndex();
 }

 public void setInstruments(String[] s) {
  for(int i=0; i<s.length; i++) instruCB.addItem(s[i]);
 }

}
