#include "NGLScene.h"
#include <QGuiApplication>
#include <QMouseEvent>

#include <ngl/Camera.h>
#include <ngl/Light.h>
#include <ngl/Material.h>
#include <ngl/NGLInit.h>
#include <ngl/NGLStream.h>
#include <ngl/ShaderLib.h>
#include <ngl/VAOPrimitives.h>
#include <ngl/Transformation.h>


NGLScene::NGLScene()
{
    setTitle( "Qt5 Simple NGL Demo" );
}


NGLScene::~NGLScene()
{
    std::cout << "Shutting down NGL, removing VAO's and Shaders\n";
}



void NGLScene::resizeGL( int _w, int _h )
{
    m_cam.setShape( 45.0f, static_cast<float>( _w ) / _h, 0.05f, 350.0f );
    m_win.width  = static_cast<int>( _w * devicePixelRatio() );
    m_win.height = static_cast<int>( _h * devicePixelRatio() );
}


void NGLScene::initializeGL()
{
    // we must call that first before any other GL commands to load and link the
    // gl commands from the lib, if that is not done program will crash
    ngl::NGLInit::instance();
    glClearColor( 0.4f, 0.4f, 0.4f, 1.0f ); // Grey Background
    // enable depth testing for drawing
    glEnable( GL_DEPTH_TEST );
    // enable multisampling for smoother drawing
#ifndef USINGIOS_
    glEnable( GL_MULTISAMPLE );
#endif
    // now to load the shader and set the values
    // grab an instance of shader manager
    ngl::ShaderLib* shader = ngl::ShaderLib::instance();
    // we are creating a shader called Phong to save typos
    // in the code create some constexpr
    constexpr auto shaderProgram = "Phong";
    constexpr auto vertexShader  = "PhongVertex";
    constexpr auto fragShader    = "PhongFragment";
    // create the shader program
    shader->createShaderProgram( shaderProgram );
    // now we are going to create empty shaders for Frag and Vert
    shader->attachShader( vertexShader, ngl::ShaderType::VERTEX );
    shader->attachShader( fragShader, ngl::ShaderType::FRAGMENT );
    // attach the source
    shader->loadShaderSource( vertexShader, "shaders/PhongVertex.glsl" );
    shader->loadShaderSource( fragShader, "shaders/PhongFragment.glsl" );
    // compile the shaders
    shader->compileShader( vertexShader );
    shader->compileShader( fragShader );
    // add them to the program
    shader->attachShaderToProgram( shaderProgram, vertexShader );
    shader->attachShaderToProgram( shaderProgram, fragShader );


    // now we have associated that data we can link the shader
    shader->linkProgramObject( shaderProgram );
    // and make it active ready to load values
    ( *shader )[ shaderProgram ]->use();
    // the shader will use the currently active material and light0 so set them
    ngl::Material m( ngl::STDMAT::GOLD );
    // load our material values to the shader into the structure material (see Vertex shader)
    m.loadToShader( "material" );
    // Now we will create a basic Camera from the graphics library
    // This is a static camera so it only needs to be set once
    // First create Values for the camera position
    ngl::Vec3 from( 1, 1, 1 );
    ngl::Vec3 to( 0, 0, 0 );
    ngl::Vec3 up( 0, 1, 0 );
    // now load to our new camera
    m_cam.set( from, to, up );
    // set the shape using FOV 45 Aspect Ratio based on Width and Height
    // The final two are near and far clipping planes of 0.5 and 10
    m_cam.setShape( 45.0f, 720.0f / 576.0f, 0.05f, 350.0f );
    shader->setUniform( "viewerPos", m_cam.getEye().toVec3() );
    // now create our light that is done after the camera so we can pass the
    // transpose of the projection matrix to the light to do correct eye space
    // transformations
    ngl::Mat4 iv = m_cam.getViewMatrix();
    iv.transpose();
    ngl::Light light( ngl::Vec3( 5, 0, 0 ), ngl::Colour( 1, 1, 1, 1 ), ngl::Colour( 0, 1, 1, 1 ), ngl::LightModes::POINTLIGHT );
    ngl::Light light1( ngl::Vec3( 0, 5, 0 ), ngl::Colour( 1, 1, 1, 1 ), ngl::Colour( 1, 1, 1, 1 ), ngl::LightModes::POINTLIGHT );
    ngl::Light light2( ngl::Vec3( 0, 0, 5 ), ngl::Colour( 1, 1, 1, 1 ), ngl::Colour( 1, 1, 1, 1 ), ngl::LightModes::POINTLIGHT );
    ngl::Light light3( ngl::Vec3( -5, 0, 0 ), ngl::Colour( 1, 1, 1, 1 ), ngl::Colour( 1, 1, 1, 1 ), ngl::LightModes::POINTLIGHT );
    ngl::Light light4( ngl::Vec3( 0, -5, 0 ), ngl::Colour( 1, 1, 1, 1 ), ngl::Colour( 1, 1, 1, 1 ), ngl::LightModes::POINTLIGHT );
    ngl::Light light5( ngl::Vec3( 0, 0, -5 ), ngl::Colour( 1, 1, 1, 1 ), ngl::Colour( 1, 1, 1, 1 ), ngl::LightModes::POINTLIGHT );


    light.setTransform( iv );
    light1.setTransform( iv );
    light2.setTransform( iv );
    light3.setTransform( iv );
    light4.setTransform( iv );
    light5.setTransform( iv );
    // load these values to the shader as well
    light.loadToShader( "light" );
    light1.loadToShader( "light" );
    light2.loadToShader( "light" );
    light3.loadToShader( "light" );
    light4.loadToShader( "light" );
    light5.loadToShader( "light" );

    MakeArms(4);
    //Update();
}



void NGLScene::loadMatricesToShader()
{
    ngl::ShaderLib* shader = ngl::ShaderLib::instance();

    ngl::Mat4 MV;
    ngl::Mat4 MVP;
    ngl::Mat3 normalMatrix;
    ngl::Mat4 M;
    M            = m_mouseGlobalTX*m_transform.getMatrix();
    MV           = m_cam.getViewMatrix() * M;
    MVP          = m_cam.getVPMatrix() * M;

    normalMatrix = MV;
    normalMatrix.inverse().transpose();
    shader->setUniform( "MV", MV );
    shader->setUniform( "MVP", MVP );
    shader->setUniform( "normalMatrix", normalMatrix );
    shader->setUniform( "M", M );
}

void NGLScene::paintGL()
{
    glViewport( 0, 0, m_win.width, m_win.height );
    // clear the screen and depth buffer
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    drawScene("Phong");
}
//----------------------------------------------------------------------------------------------------------------------

void NGLScene::drawScene(const std::string &_shader)
{
  // grab an instance of the shader manager
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();
  (*shader)[_shader]->use();
  // clear the screen and depth buffer
  // Rotation based on the mouse position for our global
  // transform
  ngl::Mat4 rotX;
  ngl::Mat4 rotY;
  ngl::Mat4 rotZ;
  // create the rotation matrices
  rotX.rotateX(m_win.spinXFace);
  rotY.rotateY(m_win.spinYFace);
  // multiply the rotations
  m_mouseGlobalTX=rotY*rotX;
  // add the translations
  m_mouseGlobalTX.m_m[3][0] = m_modelPos.m_x;
  m_mouseGlobalTX.m_m[3][1] = m_modelPos.m_y;
  m_mouseGlobalTX.m_m[3][2] = m_modelPos.m_z;

   // get the VBO instance and draw the built in teapot
  ngl::VAOPrimitives *prim=ngl::VAOPrimitives::instance();
  prim->createTrianglePlane("plane",10,10,1,1,ngl::Vec3(0,0,0));

  prim->draw("plane");
  loadMatricesToShader();

  m_arms[1].m_rot=ngl::Vec3(-90,0,0);
  m_arms[1].m_pos=ngl::Vec3(0,1,0);

  for(size_t i = 0; i<m_arms.size(); ++i)
  {

      m_arms[i].m_scale=ngl::Vec3(0.2,0.2,0.2);
      m_transform.reset();
      {
          m_transform.setRotation(m_arms[i].m_rot.m_x, m_arms[i].m_rot.m_y, m_arms[i].m_rot.m_z);
          m_transform.setPosition(m_arms[i].m_pos.m_x, m_arms[i].m_pos.m_y, m_arms[i].m_pos.m_z);
          m_transform.setScale(m_arms[i].m_scale.m_x, m_arms[i].m_scale.m_y, m_arms[i].m_scale.m_z);
          Update(i);
        loadMatricesToShader();
        m_arms[i].m_vao->draw();
      } // and before a pop

  }

}
//----------------------------------------------------------------------------------------------------------------------
void NGLScene::MakeArms(int m_num)
{
    for ( int i = 0; i < m_num; ++i )
    {
        m_arms.emplace_back();
        m_arms[i].buildVAO();
    }
}

void NGLScene::Tranform()
{


    m_arms[1].verts[1]=ngl::Vec3(1,1,1);


}

void NGLScene::Update(size_t _id)
{
    //for ( int i = 0; i < m_arms.size(); ++i )
    //{
        m_arms[_id].m_vao.reset(ngl::VAOFactory::createVAO(ngl::simpleVAO,GL_TRIANGLES) );
        m_arms[_id].m_vao->bind();
        //ngl::Transformation::setPosition();
        // in this case we are going to set our data as the vertices above
        auto dataSize = sizeof(m_arms[_id].verts[0]);
        m_arms[_id].m_vao->setData(ngl::SimpleVAO::VertexData( m_arms[_id].verts.size()*dataSize, m_arms[_id].verts[0].u));
        //m_vao->setData(ngl::SimpleVAO::NormalData(verts.size()*sizeof(ngl::Vec3),verts[0].m_x));
        // now we set the attribute pointer to be 0 (as this matches vertIn in our shader)

        m_arms[_id].m_vao->setVertexAttributePointer(0,3,GL_FLOAT,dataSize,5);
        // uv same as above but starts at 0 and is attrib 1 and only u,v so 2
        m_arms[_id].m_vao->setVertexAttributePointer(1,2,GL_FLOAT,dataSize,0);
        // normal same as vertex only starts at position 2 (u,v)-> nx
        m_arms[_id].m_vao->setVertexAttributePointer(2,3,GL_FLOAT,dataSize,2);

        // divide by 2 as we have both verts and normals
        m_arms[_id].m_vao->setNumIndices( m_arms[_id].verts.size());

        // now unbind
        //m_arms[i]. m_vao->unbind();
    //}
}

//----------------------------------------------------------------------------------------------------------------------

void NGLScene::keyPressEvent( QKeyEvent* _event )
{
    // that method is called every time the main window recives a key event.
    // we then switch on the key value and set the camera in the GLWindow
    switch ( _event->key() )
    {
    // escape key to quit
    case Qt::Key_Escape:
        QGuiApplication::exit( EXIT_SUCCESS );
        break;
        // turn on wirframe rendering
#ifndef USINGIOS_
    case Qt::Key_W:
        glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
        break;
        // turn off wire frame
    case Qt::Key_S:
        glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
        break;
#endif
        // show full screen
    case Qt::Key_F:
        showFullScreen();
        break;
        // show windowed
    case Qt::Key_N:
        showNormal();
        break;
    case Qt::Key_Space :
        m_win.spinXFace=0;
        m_win.spinYFace=0;
        m_modelPos.set(ngl::Vec3::zero());
        break;
    default:
        break;
    }
    update();
}
