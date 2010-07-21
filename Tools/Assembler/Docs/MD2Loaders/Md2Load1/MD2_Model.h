//---------------------------------------------------------------------------
/// \file	MD2_Model.h
/// \date	3-Jan-2005
/// \author	Rob Bateman	[robthebloke@hotmail.com]
/// \brief	This file defines propbably the simplest going MD2 file loader.
///			It's not as efficient as the more advanced loader on my website,
///			but it should provide a good place to understand the file format.
///			
///			The class inherits the loading capabilities from the class MD2. 
///			A list of anim cycle references and vertex data is stored here.
///
//---------------------------------------------------------------------------

#ifdef WIN32
	#pragma once
#endif

#ifndef MD2_SIMPLE_MODEL__H__
#define MD2_SIMPLE_MODEL__H__


	#include <vector>
	#include <string>
	#include <iostream>
	#include <string.h>
	#include <assert.h>
	#include <stdlib.h>
	#include <GL/glut.h>
	#include "MD2_File.h"

	/// if you want to ensure the model is always drawn with
	/// GL_TRIANGLES, set this to 1. If not, the model may be 
	/// rendered with triangle strips or fans. I've found that 
	/// rendering with triangles is way faster. I guess that 
	/// using glBegin & glEnd all the time slows stuff down quite 
	/// a bit :(
	#define MD2_ALWAYS_GL_TRIANGLES 0

	/// sets the frames per second that the anims occur at
	#define MD2_FRAMES_PER_SEC 5

	//-----------------------------------------------------------------------	class MD2Model
	//
	class MD2Model 
		: private MD2 {
	public:

		/// ctor
		MD2Model() ;

		/// dtor
		~MD2Model() ;
		
		/// \brief	releases the current loaded model
		///
		void Release();

		/// \brief	this func loads the specified md2 file
		/// \param	filename	-	name of the model to load
		/// \return	true if OK
		///
		bool Load(const char* filename) ;

		/// \brief	this function updates the current animation by the specified
		///			amount. 
		/// \param	dt	-	the time increment
		/// 
		void Update(float dt) ;

		/// \brief	renders the model using triangle strips if it has them,
		///			otherwise it falls back to triangles
		/// 
		void Render() const;

		/// \brief	This funcion sets the current animation to use
		/// \param	idx	-	the index of the anim to play
		/// 
		void SetAnim(unsigned short);

		/// \brief	This function returns the name of the first frame of 
		///			the specified animation, ie "run01"
		/// \param	idx	-	the animation index
		/// \return	the text name or NULL
		///
		const char* GetAnimName(unsigned short) const;

		/// \brief	This function returns the number of animation cycles contained
		///			within the md2 file
		/// \return	num of anims
		/// 
		unsigned short GetNumAnims() const;

		/// \brief	This function returns the total size of the model in bytes
		/// \param	data size in bytes
		/// 
		unsigned int GetDataSize() const ;

	protected:

		/// holds a reference to an animation within the MD2 file
		struct AnimRef {

			/// ctor
			AnimRef() {}

			/// copy ctor
			AnimRef(const AnimRef& ref) 
				:	m_FrameStart(ref.m_FrameStart),
					m_FrameCount(ref.m_FrameCount){}
			
			/// first frame in cycle
			unsigned short m_FrameStart;

			/// number of frames in the cycle
			unsigned short m_FrameCount;
		};
		
		/// the time of the current anim
		float m_AnimTime;

		/// the final vertex coords
		float* m_Verts;
		
		/// the current ID of the anim running
		unsigned short m_CurrentAnim;

		/// the number of anims in the md2 
		unsigned short m_NumAnims;
		
		/// quick references for the animation cycles
		std::vector<AnimRef> m_Anims;
	};

#endif
