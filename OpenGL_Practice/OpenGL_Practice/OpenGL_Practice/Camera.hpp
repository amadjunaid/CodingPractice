#include <glm/glm.hpp>"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

/**
 * \brief 
 */
class Camera
{
public:
	Camera():
    m_pos(glm::vec3(0.f, 0.f, 5.f)),
    m_front(  glm::vec3(0.f, -1.f, -1.f)),
    m_up( glm::vec3(0.f, 1.f, 0.f)),
    m_fov ( 45.f),
    m_pitch ( 0.f),
    m_yaw ( -90.f)
	{        
	}


	Camera(const glm::vec3& camerPos_, const glm::vec3& cameraFront_, const glm::vec3& cameraUp_, float fov_)
		: m_pos(camerPos_),
		  m_front(cameraFront_),
		  m_up(cameraUp_),
		  m_fov(fov_)
	{
	}
        
    void moveForward(float deltaAmount)
	{
        float cameraSpeed = 2.5f  *deltaAmount;
        m_pos += cameraSpeed * m_front;
	}

    void moveBackward(float deltaAmount)
    {
        float cameraSpeed = 2.5f  *deltaAmount;
        m_pos -= cameraSpeed * m_front;
    }
    
    void moveLeft(float deltaAmount)
    {
        float cameraSpeed = 2.5f  *deltaAmount;
        m_pos -= glm::normalize(glm::cross(m_front, m_up))*cameraSpeed;
    }
    
    void moveRight(float deltaAmount)
    {
        float cameraSpeed = 2.5f  *deltaAmount;
        m_pos += glm::normalize(glm::cross(m_front, m_up))*cameraSpeed;
    }

	void rotate(double xOffset_, double yOffset_)
	{
		m_yaw += xOffset_;
		m_pitch += yOffset_;

		if (m_pitch > 89.f)
			m_pitch = 89.f;

		if (m_pitch < -89.f)
			m_pitch = -89.f;

		glm::vec3 front;
		front.x = cos(glm::radians(m_pitch)) * cos(glm::radians(m_yaw));
		front.y = sin(glm::radians(m_pitch));
		front.z = cos(glm::radians(m_pitch)) * sin(glm::radians(m_yaw));
		m_front = glm::normalize(front);
	}

	void zoom(double xoffset, double yoffset )
	{
		if (m_fov >= 1.0f && m_fov <= 45.0f)
			m_fov -= yoffset;
		if (m_fov <= 1.0f)
			m_fov = 1.0f;
		if (m_fov >= 45.0f)
			m_fov = 45.0f;		
	}


	glm::vec3 m_pos;
	glm::vec3 m_front;
	glm::vec3 m_up;
	float m_fov;

	//Controls
	float m_pitch;
	float m_yaw;



};


