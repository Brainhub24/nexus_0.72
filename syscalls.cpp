// GPL License - see http://opensource.org/licenses/gpl-license.php
// Copyright 2006 *nixCoders team - don't forget to credit us

#include "nexus.h"

CNexSyscalls nexSyscall;

void CNexSyscalls::Cvar_Register(vmCvar_t *vmCvar, const char *varName, const char *defaultValue, int flags) {
	orig_syscall(CG_CVAR_REGISTER, vmCvar, varName, defaultValue, flags);
}

void CNexSyscalls::Cvar_Set(const char *var_name, const char *value) {
	orig_syscall(CG_CVAR_SET, var_name, value);

}

void CNexSyscalls::Cvar_Update(vmCvar_t *vmCvar) {
	orig_syscall(CG_CVAR_UPDATE, vmCvar);

}

void CNexSyscalls::Cvar_VariableStringBuffer(const char *var_name, char *buffer, int bufsize) {
	orig_syscall(CG_CVAR_VARIABLESTRINGBUFFER, var_name, buffer, bufsize);

}

void CNexSyscalls::R_AddRefEntityToScene(const refEntity_t *re) {
	orig_syscall(CG_R_ADDREFENTITYTOSCENE, re);
}

void CNexSyscalls::R_DrawStretchPic(float x, float y, float w, float h, float s1, float t1, float s2, float t2, qhandle_t hShader) {
	orig_syscall(CG_R_DRAWSTRETCHPIC, PASSFLOAT(x), PASSFLOAT(y), PASSFLOAT(w), PASSFLOAT(h), PASSFLOAT(s1), PASSFLOAT(t1), PASSFLOAT(s2), PASSFLOAT(t2), hShader);
}

void CNexSyscalls::R_SetColor(const float *rgba) {
	orig_syscall(CG_R_SETCOLOR, rgba);
}

void CNexSyscalls::Cmd_RemoveCommand(const char *cmdName) {
	orig_syscall(CG_REMOVECOMMAND, cmdName);
}


qhandle_t CNexSyscalls::R_RegisterShader(const char *name) {
	return orig_syscall(CG_R_REGISTERSHADER, name);
}

qhandle_t CNexSyscalls::R_RegisterShaderNoMip(const char *name) {
	return orig_syscall(CG_R_REGISTERSHADERNOMIP, name);
}

sfxHandle_t	CNexSyscalls::S_RegisterSound(const char *sample, qboolean compressed) {
	return orig_syscall(CG_S_REGISTERSOUND, sample, qfalse);
}

void CNexSyscalls::S_StartLocalSound(sfxHandle_t sfx, int channelNum) {
	orig_syscall(CG_S_STARTLOCALSOUND, sfx, channelNum, 230);
}

int CNexSyscalls::R_LerpTag(orientation_t *tag, const refEntity_t *refent, const char *tagName, int startIndex) {
	return orig_syscall(CG_R_LERPTAG, tag, refent, tagName, startIndex);
}

void CNexSyscalls::R_ModelBounds(clipHandle_t model, vec3_t mins, vec3_t maxs) {
	orig_syscall(CG_R_MODELBOUNDS, model, mins, maxs);
}

void CNexSyscalls::R_AddLightToScene(const vec3_t org, float radius,float intensity, float r, float g, float b, qhandle_t hShader, int flags) {
	orig_syscall(CG_R_ADDLIGHTTOSCENE, org, PASSFLOAT(radius), PASSFLOAT(intensity), PASSFLOAT(r), PASSFLOAT(g), PASSFLOAT(b), hShader, flags);
}

void CNexSyscalls::R_ProjectDecal(qhandle_t hShader,int numPoints, vec3_t *points,vec4_t projection, vec4_t color,int lifeTime, int fadeTime) {
	orig_syscall(CG_R_PROJECTDECAL, hShader, numPoints, points, projection, color, lifeTime, fadeTime);

}

void CNexSyscalls::SendConsoleCommand(const char *text) {
	orig_syscall(CG_SENDCONSOLECOMMAND, text);
}

bool CNexSyscalls::CG_Key_IsDown(int keynum) {
	return orig_syscall(CG_KEY_ISDOWN, keynum);
}

const char *CNexSyscalls::UI_Argv(int arg) {
	static char buffer[MAX_STRING_CHARS];
	orig_syscall(UI_ARGV, arg, buffer, sizeof(buffer));
	return buffer;
}

int CNexSyscalls::UI_Argc() {
	return orig_syscall(UI_ARGC);
}

const char *CNexSyscalls::CG_Argv(int arg) {
	static char buffer[MAX_STRING_CHARS];
	orig_syscall(CG_ARGV, arg, buffer, sizeof buffer);
	return buffer;
}

int CNexSyscalls::CG_Argc() {
	return orig_syscall(CG_ARGC);
}

void CNexSyscalls::UI_Print(const char *txt) {
	orig_syscall(UI_PRINT, va("%s\n", txt));
}

bool CNexSyscalls::isKeyActionDown(char *action) {
	int key1, key2;
	orig_syscall(CG_KEY_BINDINGTOKEYS, action, &key1, &key2);

	if (nexSyscall.CG_Key_IsDown(key1) || nexSyscall.CG_Key_IsDown(key2))
		return true;
	else
		return false;
}
