unit MPPT;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, StdCtrls, ExtCtrls,strutils, TeeProcs, TeEngine, Chart, CPort, mmsystem,
  Series, Buttons, jpeg;

type
  TMPPT_Aji = class(TForm)
    GroupBox1: TGroupBox;
    GroupBox2: TGroupBox;
    Label2: TLabel;
    Label3: TLabel;
    Label4: TLabel;
    Label5: TLabel;
    Label6: TLabel;
    Label7: TLabel;
    Label12: TLabel;
    Label13: TLabel;
    Label14: TLabel;
    Label15: TLabel;
    Label16: TLabel;
    Label17: TLabel;
    Label18: TLabel;
    Button1: TButton;
    Button2: TButton;
    Button3: TButton;
    Image1: TImage;
    Label1: TLabel;
    Label32: TLabel;
    Label11: TLabel;
    Memo1: TMemo;
    ComPort1: TComPort;
    GroupBox3: TGroupBox;
    Label22: TLabel;
    Label23: TLabel;
    Label24: TLabel;
    Label25: TLabel;
    Label26: TLabel;
    Label27: TLabel;
    Label28: TLabel;
    Label29: TLabel;
    Label30: TLabel;
    Label31: TLabel;
    Label8: TLabel;
    Label9: TLabel;
    Label10: TLabel;
    Label19: TLabel;
    Label20: TLabel;
    Label21: TLabel;
    procedure Button1Click(Sender: TObject);
    procedure Button2Click(Sender: TObject);
    procedure Button3Click(Sender: TObject);
    procedure FormCreate(Sender: TObject);
    procedure ComPort1RxChar(Sender: TObject; Count: Integer);
    procedure Memo1Change(Sender: TObject);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  MPPT_Aji: TMPPT_Aji;

implementation

{$R *.dfm}

procedure TMPPT_Aji.Button1Click(Sender: TObject);
begin
   comport1.ShowSetupDialog;
end;

procedure TMPPT_Aji.Button2Click(Sender: TObject);
begin
   if button2.Caption='Connect' then
   begin
   button2.Caption:='Disconnect';
   comport1.Open;
   end
else if button2.Caption='Disconnect' then
   begin
   button2.Caption:='Connect';
   comport1.Close;
   end;
end;

procedure TMPPT_Aji.Button3Click(Sender: TObject);
begin
   Label3.Caption:='-';
   Label6.Caption:='-';
   Label9.Caption:='-';
   Label15.Caption:='-';
   Label17.Caption:='-';
   Label20.Caption:='-';
   Label23.Caption:='-';
   Label26.Caption:='-';
   Label29.Caption:='-';
end;

procedure TMPPT_Aji.FormCreate(Sender: TObject);
begin
   ThousandSeparator:=',';
   DecimalSeparator:='.';
end;

procedure TMPPT_Aji.ComPort1RxChar(Sender: TObject; Count: Integer);
var buff:string;
begin
comport1.ReadStr(buff,count);
memo1.Text:=memo1.Text+buff;

end;


procedure TMPPT_Aji.Memo1Change(Sender: TObject);
var V_PV,I_PV,P_PV,Load,PWM,EFF,V_BAT,I_BAT,P_BAT:string;

begin

  P_BAT:=memo1.Lines[memo1.Lines.count-9];
  I_BAT:=memo1.Lines[memo1.Lines.count-8];
  V_BAT:=memo1.Lines[memo1.Lines.count-7];
  EFF:=memo1.Lines[memo1.Lines.count-6];
  PWM:=memo1.Lines[memo1.Lines.count-5];
  Load:=memo1.Lines[memo1.Lines.count-4];
  P_PV:=memo1.Lines[memo1.Lines.count-3];
  I_PV:=memo1.Lines[memo1.Lines.count-2];
  V_PV:=memo1.Lines[memo1.Lines.count-1];

  if rightstr(V_PV,1)= 'a' then
     begin
     Label3.Caption := leftstr(V_PV,length(V_PV)-1);
     end;
  if rightstr(I_PV,1)='b' then
     begin
     Label6.Caption := leftstr(I_PV,length(I_PV)-1);
     end;
  if rightstr(P_PV,1)='c' then
     begin
     Label9.Caption := leftstr(P_PV,length(P_PV)-1);
     end;
  if rightstr(Load,1)='d' then
     begin
     Label15.Caption := leftstr(Load,length(Load)-1);
     end;
  if rightstr(PWM,1)='e' then
     begin
     Label17.Caption := leftstr(PWM,length(PWM)-1);
     end;        
  if rightstr(EFF,1)='f' then
     begin
     Label20.Caption := leftstr(EFF,length(EFF)-1);
     end;
  if rightstr(V_BAT,1)='g' then
     begin
     Label23.Caption := leftstr(V_BAT,length(V_BAT)-1);
     end;
  if rightstr(I_BAT,1)='h' then
     begin
     Label26.Caption := leftstr(I_BAT,length(I_BAT)-1);
     end;
  if rightstr(P_BAT,1)='i' then
     begin
     Label29.Caption := leftstr(P_BAT,length(P_BAT)-1);
     end;

end;

end.


