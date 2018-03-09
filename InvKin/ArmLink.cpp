#include "ArmLink.h"
#include "ngl/SimpleVAO.h"
#include "ngl/VAOFactory.h"

void ArmLink::buildVAO()
{


    verts={
        ngl::Vec3(0,0,0),
        ngl::Vec3(0,1,0),
        ngl::Vec3(0.5,0.5,2),
        ngl::Vec3(0,0,0),
        ngl::Vec3(1,0,0),
        ngl::Vec3(0.5,0.5,2),
        ngl::Vec3(1,0,0),
        ngl::Vec3(1,1,0),
        ngl::Vec3(0.5,0.5,2),
        ngl::Vec3(1,1,0),
        ngl::Vec3(0,1,0),
        ngl::Vec3(0.5,0.5,2),
        ngl::Vec3(0,0,0),
        ngl::Vec3(0,1,0),
        ngl::Vec3(0.5,0.5,-1),
        ngl::Vec3(0,0,0),
        ngl::Vec3(1,0,0),
        ngl::Vec3(0.5,0.5,-1),
        ngl::Vec3(1,0,0),
        ngl::Vec3(1,1,0),
        ngl::Vec3(0.5,0.5,-1),
        ngl::Vec3(1,1,0),
        ngl::Vec3(0,1,0),
        ngl::Vec3(0.5,0.5,-1)
    };


    /*for(int i;i<verts.size();i+3)
    {
        transform.addPosition(verts[i].x,verts[i].y,verts[i].z);
    }*/

    for (int i = 0; i < verts.size() / 3; ++i)
    {
        auto& p1 = verts[i * 3];
        auto& p2 = verts[i * 3 + 1];
        auto& p3 = verts[i * 3 + 2];

        auto v1 = ngl::Vec3(p1.x, p1.y, p1.z);
        auto v2 = ngl::Vec3(p2.x, p2.y, p2.z);
        auto v3 = ngl::Vec3(p3.x, p3.y, p3.z);
        //calc norm
        ngl::Vec3 a= v2 - v1;
        ngl::Vec3 b= v3 - v1;
        auto normal =a*b;
        p1.nx =normal[0] ;
        p1.ny =normal[1] ;
        p1.nz =normal[2] ;

        p2.nx =normal[0];
        p2.ny =normal[1];
        p2.nz =normal[2];

        p3.nx =normal[0];
        p3.ny =normal[1];
        p3.nz =normal[2];
    }

    std::cout<<"sizeof(verts) "<<sizeof(verts)<<" sizeof(ngl::Vec3) "<<sizeof(ngl::Vec3)<<"\n";
    std::cout<<"sizeof(verts) "<<sizeof(verts)<<" sizeof(ngl::Vec3) "<<sizeof(ngl::Vec3)<<"\n";
    // create a vao as a series of GL_TRIANGLES
    m_vao.reset(ngl::VAOFactory::createVAO(ngl::simpleVAO,GL_TRIANGLES) );
    m_vao->bind();
    //ngl::Transformation::setPosition();
    // in this case we are going to set our data as the vertices above
    m_vao->setData(ngl::SimpleVAO::VertexData(verts.size()*sizeof(vertData),verts[0].u));
    //m_vao->setData(ngl::SimpleVAO::NormalData(verts.size()*sizeof(ngl::Vec3),verts[0].m_x));
    // now we set the attribute pointer to be 0 (as this matches vertIn in our shader)

    m_vao->setVertexAttributePointer(0,3,GL_FLOAT,sizeof(vertData),5);
    // uv same as above but starts at 0 and is attrib 1 and only u,v so 2
    m_vao->setVertexAttributePointer(1,2,GL_FLOAT,sizeof(vertData),0);
    // normal same as vertex only starts at position 2 (u,v)-> nx
    m_vao->setVertexAttributePointer(2,3,GL_FLOAT,sizeof(vertData),2);

    // divide by 2 as we have both verts and normals
    m_vao->setNumIndices(verts.size());

    // now unbind
    m_vao->unbind();
}

/*void ArmLink::updateMat()
{
    ngl::Mat4 tmp = ngl::Mat4(1);
    m_matrix.rotateX(m_rot.m_x);
    m_matrix.rotateY(m_rot.m_y);
    m_matrix.rotateZ(m_rot.m_z);
    m_matrix.translate(m_pos.m_x, m_pos.m_y, m_pos.m_z);
}

ngl::Mat4 ArmLink::getMat()
{
    return m_matrix;
}*/
