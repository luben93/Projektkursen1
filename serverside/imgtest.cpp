int main(){

ILubyte *Lump;
ILuint Size;
FILE *File;

File = fopen("monkey.tga", "rb");
fseek(File, 0, SEEK_END);
Size = ftell(File);

Lump = (ILubyte*)malloc(Size);
fseek(File, 0, SEEK_SET);
fread(Lump, 1, Size, File);
fclose(File);

ilLoadL(IL_TGA, Lump, Size);
free(Lump);
return 0;
}