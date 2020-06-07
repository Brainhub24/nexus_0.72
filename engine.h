// GPL License - see http://opensource.org/licenses/gpl-license.php
// Copyright 2006 *nixCoders team - don't forget to credit us

#pragma once

class CNexEngine {
	public:
		void prev_CG_Init();
		void post_CG_Init();
		void prev_CG_DrawActiveFrame();
		void post_CG_DrawActiveFrame();
		void CG_Shutdown();
		void CG_R_AddRefEntityToScene(refEntity_t *re);
		void CG_S_UpdateEntityPosition(int entityNum, const vec3_t origin);
		bool CG_R_AddPolyToScene(qhandle_t shader, polyVert_t *polyVert);
		bool CG_R_RenderScene(refdef_t *refdef);
		bool CG_R_DrawStretchPic(float x, float y, float w, float h, float s1, float t1, float s2, float t2, qhandle_t hShader);
		void unreferenceBadPk3();
		void registerMedias();
		bool isVisible(vec3_t target);
		bool isPlayerVisible(vec3_t target, int player);
		void setMuzzle();
		int getSpawntime(bool enemySpawn);
		int findSatchel();
	private:
};

extern CNexEngine nexEngine;
