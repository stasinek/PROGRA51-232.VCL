#include <vcl.h>
#include "unit2.h"
#include "Unit3.h"
#pragma hdrstop
#include "Unit1.h"
#include "./../../FORM_TEMPLATES/About_frm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
#define STR_TOO_SMALL "Program won't fit into this microcontroller!"
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
        : TForm(Owner)
{
Flash51->OnBreak = Form1->DoBreak;
}
//---------------------------------------------------------------------------

__int32 __stdcall TForm1::DoBreak(__int32 brParam)
{
ProgressBar1->Position = 100 * (float(Flash51->plik.Pos) / float(Flash51->plik.Size));
Application->ProcessMessages();
return brParam;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::FormCreate(TObject *Sender)
{
int i, s, x;
char name[256];
strcpy(name,GetCommandLine());
for (i = 0, x = 0, s = strlen(name); i < s; i++)
    {if (name[i]=='\"')
        {if (x==3) break;
         x++;
         strcpy(name,&name[i+1]);
         s-=i+1;
         i=0;
        }
    }
for (s = strlen(name), i = s; i >=0; i--)
    {if (name[i]=='\"')
        {name[i] ='\0';
        }
    }
Flash51->plik.Open(name);
Edit1->Text = Flash51->plik.Path;

Label1->Caption = "Program size = ???? B FLASH = ???? B EEPROM = ???? B";

ComboBox1->ItemIndex = 0;
ComboBox1Change(Sender);
ComboBox2->ItemIndex = 0;
ComboBox2Change(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::FormClose(TObject *Sender, TCloseAction &Action)
{
Flash51->Stop = true;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::BitBtn1Click(TObject *Sender)
{
if (Flash51->FLASH < Flash51->plik.Size)
   {StatusBar1->SimpleText = STR_TOO_SMALL;
   }
else
   {Form1->Enabled = false;
    if (Flash51->Prog())
    StatusBar1->SimpleText = "Flashing finished with SUCCESS!";
    else StatusBar1->SimpleText = "Flashing FAILED!";
    Form1->Enabled = true;
   }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::BitBtn2Click(TObject *Sender)
{
if (OpenDialog1->Execute())
   {
    Edit1->Text = OpenDialog1->FileName;

    if (Flash51->plik.Open(Edit1->Text.c_str()))
       {ComboBox1Change(Sender);
       }
    else
       {Flash51->OnError(0);
       }
   }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ComboBox1Change(TObject *Sender)
{
if (ComboBox1->ItemIndex==2) Flash51->FLASH = AT89C1024;
if (ComboBox1->ItemIndex==1) Flash51->FLASH = AT89C2048;
if (ComboBox1->ItemIndex==0) Flash51->FLASH = AT89C4096;

Label1->Caption = "Program size : " + (String)Flash51->plik.Size + " B, FLASH : " + (String)Flash51->FLASH + " B";

ProgressBar2->Position = 100 * (float)Flash51->plik.Size / (float)Flash51->FLASH;

if (Flash51->FLASH < Flash51->plik.Size)
   {StatusBar1->SimpleText = STR_TOO_SMALL;
   }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::BitBtn3Click(TObject *Sender)
{
if (Flash51->FLASH < Flash51->plik.Size)
   {StatusBar1->SimpleText = STR_TOO_SMALL;
   }
else
   {
    
    Form2->ShowModal();
   }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ComboBox2Change(TObject *Sender)
{
if (ComboBox2->ItemIndex==0) Flash51->LOCKBIT = 0;
if (ComboBox2->ItemIndex==1) Flash51->LOCKBIT = 1;
if (ComboBox2->ItemIndex==2) Flash51->LOCKBIT = 2;
if (ComboBox2->ItemIndex==3) Flash51->LOCKBIT = 3;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::SpeedButton1Click(TObject *Sender)
{
StatusBar1->SimpleText = "Programowano " + (String)*Flash51->Eezap();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::SpeedButton2Click(TObject *Sender)
{
char *buff = Flash51->Sign();
StatusBar1->SimpleText = (String)(int)buff[0] + "," + (String)(int)buff[1] + ","  + (String)(int)buff[2];
}
//---------------------------------------------------------------------------

void __fastcall TForm1::SpeedButton3Click(TObject *Sender)
{
Flash51->Erase();
StatusBar1->SimpleText = "Procesor ERASED!";
}
//---------------------------------------------------------------------------

void __fastcall TForm1::SpeedButton4Click(TObject *Sender)
{
Flash51->Lock();
StatusBar1->SimpleText = "Procesor LOCKED!";
}
//---------------------------------------------------------------------------

void __fastcall TForm1::SpeedButton5Click(TObject *Sender)
{
Flash51->Read();
StatusBar1->SimpleText = "Content of ROM in in buffer.";
}
//---------------------------------------------------------------------------

void __fastcall TForm1::SpeedButton6Click(TObject *Sender)
{
Flash51->Reset();
StatusBar1->SimpleText = "Programmer RESET!";
}
//---------------------------------------------------------------------------



void __fastcall TForm1::SpeedButton7Click(TObject *Sender)
{
About_form->ShowModal();    
}
//---------------------------------------------------------------------------


