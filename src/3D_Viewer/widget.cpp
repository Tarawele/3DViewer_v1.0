#include "widget.h"

widget::widget() { setWindowTitle("3D_Viewer"); }

widget::~widget() {
  if (model) res_remove(model);
}

void widget::paintGL() {
  //цвет фона
  glClearColor(back_red, back_green, back_blue,
               1);  //сюда подставить значения, который задает пользователь
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  if (model) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //тип модели
    if (!progection_mode)
      glOrtho(-1.5, 1.5, -1.5, 1.5, -2, 1000);
    else {
      glFrustum(-1.5, 1.5, -1.5, 1.5, 0.8, 1000);
      glTranslatef(0, 0, -2);
    }
    //тут отрисовка
    glEnableClientState(GL_VERTEX_ARRAY);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    if (rib_type) {
      glEnable(GL_LINE_STIPPLE);  // рисование пунктиром
      glLineStipple(1, 0x00F0);
    } else {
      glDisable(GL_LINE_STIPPLE);  // возврат линий
    }
    glLineWidth(rib_width);
    glColor3d(rib_red, rib_green, rib_blue);
    glVertexPointer(3, GL_DOUBLE, 0, model->points);
    glDrawElements(GL_LINES, model->amount_connections, GL_UNSIGNED_INT,
                   model->connections);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    if (dot_type) {
      if (dot_type == 1)
        glEnable(GL_POINT_SMOOTH);
      else
        glDisable(GL_POINT_SMOOTH);
      glPointSize(dot_width);
      glColor3d(dot_red, dot_green, dot_blue);
      glDrawArrays(GL_POINTS, 0, model->amount_points / 3);
    }
    //    glColor3d(2/ 100, 50 / 100, 100/ 100);
    //    glDrawArrays(GL_POINTS, 0, model->amount_points);
    glDisable(GL_LINE_STIPPLE);
    glDisable(GL_POINT_SMOOTH);
    glDisableClientState(GL_VERTEX_ARRAY);
  }
}

void widget::initializeGL() {
  model = NULL;
  progection_mode = 0;
  dot_type = 0;
  rib_type = 0;
  rib_width = 1;
  dot_width = 1;
  back_red = 0.;
  back_green = 0.;
  back_blue = 0.;
  rib_red = 255.;
  rib_green = 255.;
  rib_blue = 255.;
  dot_red = 255.;
  dot_green = 0.;
  dot_blue = 0.;
  glEnable(GL_DEPTH_TEST);
}
