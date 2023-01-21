#include "bspline.hh"

#include <fstream>

glm::vec3 parseVec3(std::string &str)
{
    std::string::iterator it = str.begin();
    std::vector<float> ret;
    std::string temp = "";
    for (int i = 0; i < str.size(); ++i)
    {
        if (str[i] != ' ')
        {
            temp += str[i];
        }
        else if (temp.size() > 0)
        {
            ret.emplace_back(std::stof(temp));
            temp = "";
        }
    }
    if (temp.size() > 0)
        ret.emplace_back(std::stof(temp));
    // std::cout << ret[0] << "." << ret[1] << "." << ret[2] << "\n";
    return glm::vec3{ret[0], ret[1], ret[2]};
}

Bspline::Bspline(std::string &&fileToControlPoints)
{
    std::ifstream file{fileToControlPoints};

    if (!file.good())
    {
        std::cerr << "Unable to open file : " << fileToControlPoints << "\n";
        exit(-1);
    }

    std::string line;
    while (std::getline(file, line))
    {
        controlPoints.emplace_back(parseVec3(line));
        ++numberOfControlPoints;
    }
    /*
        numberOfControlPoints = 4;
        controlPoints.emplace_back(0., 0., 0.);
        controlPoints.emplace_back(-1., 3., 0.);
        controlPoints.emplace_back(1., 4., 0.);
        controlPoints.emplace_back(2., 3., 0.);
     */
    // other data //

    cubicBsplineFactor = 1.0 / 6.0;
    cubicBspline = glm::transpose(
        glm::mat4x4(
            glm::vec4(-1., 3., -3., 1.),
            glm::vec4(3., -6., 3., 0.),
            glm::vec4(-3., 0., 3., 0.),
            glm::vec4(1., 4., 1., 0.)));
}

void Bspline::drawControlPoints()
{
    glBegin(GL_LINE_STRIP);

    for (auto &controlPoint : controlPoints)
        glVertex3f(controlPoint.x, controlPoint.y, controlPoint.z);

    glEnd();
}

VertexOnRoids Bspline::getPositionOnCurve(float t)
{
    int degreeOfSpline = 3;
    while (t >= (float)(numberOfControlPoints - degreeOfSpline))
    {
        t -= (float)(numberOfControlPoints - degreeOfSpline);
    }

    int segmentNo = (int)t;
    t -= (int)t;
    std::cerr << "Segment for the t = " << t << " is segmentNo = " << segmentNo << "\n";

    glm::vec4 Tvec{pow(t, 3), pow(t, 2), pow(t, 1), 1.};
    glm::vec4 Tdervec{3. * pow(t,2), 2. * t, 1., 0.};

    glm::mat4x3 currControlPoints{
        controlPoints[segmentNo],
        controlPoints[segmentNo + 1],
        controlPoints[segmentNo + 2],
        controlPoints[segmentNo + 3],
    };
    glm::mat3x4 brMat = cubicBspline * glm::transpose(currControlPoints);
    
    glm::vec3 calculatedPoint = Tvec * cubicBsplineFactor * brMat;
    glm::vec3 tangent = Tdervec * cubicBsplineFactor * brMat;

    VertexOnRoids ret;
    ret.position = calculatedPoint;
    ret.tangent = tangent;
    return ret;

    
}

void Bspline::drawBspline()
{
    const int degreeOfSpline = 3;

    glBegin(GL_LINE_STRIP);

    for (int segmentNo = 0; segmentNo < numberOfControlPoints - degreeOfSpline; ++segmentNo)
    {
        for (float t = 0; t <= 1.0; t += 0.01)
        {
            glm::vec4 Tvec{pow(t, 3), pow(t, 2), pow(t, 1), 1.};
            glm::mat4x3 currControlPoints{
                controlPoints[segmentNo],
                controlPoints[segmentNo + 1],
                controlPoints[segmentNo + 2],
                controlPoints[segmentNo + 3],
            };
            glm::mat3x4 brMat = cubicBspline * glm::transpose(currControlPoints);
            glm::vec3 calculatedPoint = Tvec * cubicBsplineFactor * brMat;
            // std::cout << "(" << calculatedPoint.x << "," << calculatedPoint.y << "," << calculatedPoint.z << ")"
            //           << "\n";
            glVertex3f(calculatedPoint.x, calculatedPoint.y, calculatedPoint.z);
        }
    }

    glEnd();
}

void Bspline::drawAll(float fake_time)
{
    glColor3f(0.0f, 1.0f, 0.5f);
    glPushMatrix();
    glTranslatef(0.5f, 0.0f, 0.0f);
    glScalef(1.0f, 1.0f, 1.0f);
    for (auto &it : Bspline::splines)
    {
        it->drawBspline();
    }
    glPopMatrix();

    glColor3f(0.0f, 0.0f, 1.0f);
    glPushMatrix();
    glTranslatef(0.5f, 0.0f, 0.0f);
    glScalef(1.0f, 1.0f, 1.0f);
    for (auto &it : Bspline::splines)
    {
        it->drawControlPoints();
    }
    glPopMatrix();

    glColor3f(1.0f, 0.0f, 0.0f);
    glPushMatrix();
    glTranslatef(0.5f, 0.0f, 0.0f);
    glScalef(1.0f, 1.0f, 1.0f);

    glBegin(GL_LINE_STRIP);
    for (auto &it : Bspline::splines)
    {
        VertexOnRoids info = it->getPositionOnCurve(fake_time);
        glVertex3f(info.position.x,info.position.y,info.position.z);
        info.tangent = info.tangent + info.position;
        glVertex3f(info.tangent.x, info.tangent.y, info.tangent.z);
    }
    glEnd();

    glPopMatrix();
}

std::vector<std::unique_ptr<Bspline>> Bspline::splines;
