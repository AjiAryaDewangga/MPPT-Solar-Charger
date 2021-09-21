program Project_MPPT;

uses
  Forms,
  MPPT in 'MPPT.pas' {MPPT_Aji};

{$R *.res}

begin
  Application.Initialize;
  Application.CreateForm(TMPPT_Aji, MPPT_Aji);
  Application.Run;
end.
