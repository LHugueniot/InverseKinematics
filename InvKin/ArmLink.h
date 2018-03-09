#ifndef ARMLINK_H
#define ARMLINK_H
#include <ngl/Camera.h>
#include <ngl/Light.h>
#include <ngl/Material.h>
#include <ngl/NGLInit.h>
#include <ngl/NGLStream.h>
#include <ngl/ShaderLib.h>
#include <ngl/VAOPrimitives.h>
#include <ngl/Transformation.h>
#include <stdlib.h>
#include <stdio.h>

class ArmLink
{
public:
    ArmLink() = default;
    ~ArmLink() = default;

    ArmLink(ArmLink&&) = default;
    ArmLink& operator = (ArmLink&&) = default;

    struct vertData
    {
      vertData(ngl::Vec3 _pos) : x(_pos.m_x),y(_pos.m_y),z(_pos.m_z) {}
      vertData() = default;
      GLfloat u;
      GLfloat v;
      GLfloat nx;
      GLfloat ny;
      GLfloat nz;
      GLfloat x;
      GLfloat y;
      GLfloat z;
    };

    std::array<vertData,24> verts;
    void buildVAO();
    ngl::Vec3 m_pos;
    ngl::Vec3 m_rot;
    ngl::Vec3 m_scale;
    float len;
    std::unique_ptr<ngl::AbstractVAO> m_vao;
    //void updateMat();
    //ngl::Mat4 getMat();
private:
    //ngl::Mat4 m_matrix;
};

#endif // ARMLINK_H
