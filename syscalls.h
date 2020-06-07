// GPL License - see http://opensource.org/licenses/gpl-license.php
// Copyright 2006 *nixCoders team - don't forget to credit us

#pragma once

class CNexSyscalls {
	public:
		void		Cvar_Register(vmCvar_t *vmCvar, const char *varName, const char *defaultValue, int flags);
		void		Cvar_Set(const char *var_name, const char *value);
		void		Cvar_Update(vmCvar_t *vmCvar);
		void		Cvar_VariableStringBuffer(const char *var_name, char *buffer, int bufsize);
		void		R_AddRefEntityToScene(const refEntity_t *re);
		void		R_DrawStretchPic(float x, float y, float w, float h, float s1, float t1, float s2, float t2, qhandle_t hShader);
		void		R_SetColor(const float *rgba);
		void		Cmd_RemoveCommand(const char *cmdName);
		qhandle_t	R_RegisterShader(const char *name);
		qhandle_t	R_RegisterShaderNoMip(const char *name);
		sfxHandle_t	S_RegisterSound(const char *sample, qboolean compressed);
		void		S_StartLocalSound(sfxHandle_t sfx, int channelNum);
		int			R_LerpTag(orientation_t *tag, const refEntity_t *refent, const char *tagName, int startIndex);
		void		R_ModelBounds(clipHandle_t model, vec3_t mins, vec3_t maxs);
		void		R_AddLightToScene(const vec3_t org, float radius,float intensity, float r, float g, float b, qhandle_t hShader, int flags);
		void		R_ProjectDecal(qhandle_t hShader,int numPoints, vec3_t *points,vec4_t projection, vec4_t color,int lifeTime, int fadeTime);
		void		SendConsoleCommand(const char *text);
		bool		CG_Key_IsDown(int keynum);
		const char	*UI_Argv(int);
		int			UI_Argc();
		const char	*CG_Argv(int);
		int			CG_Argc();
		void		UI_Print(const char *txt);
		bool		isKeyActionDown(char *action);
	private:
};

extern CNexSyscalls nexSyscall;
