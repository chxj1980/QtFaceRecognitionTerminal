#ifndef CAMERA_READER_H
#define CAMERA_READER_H

#include <QImage>
#include <QSharedPointer>
#include <QThread>
#include <chrono>

#include "config.hpp"
#include "image_package.hpp"
#include "pingpang_buffer.hpp"
#include "vi.h"
#include "vi_vpss.h"
#include "vi_vpss_vo.h"
#include "vo.h"
#include "vpss.h"

namespace suanzi {

class CameraReader : QThread {
  Q_OBJECT

 public:
  CameraReader(QObject *parent = nullptr);
  ~CameraReader();

  void start_sample();

  bool load_screen_type();
  bool get_screen_size(int &width, int &height);

 private:
 private slots:
  void rx_finish();

 signals:
  void tx_frame(PingPangBuffer<ImagePackage> *buffer);

 private:
  const HI_BOOL BGR_FLIP = HI_TRUE;
  const HI_BOOL NIR_FLIP = HI_TRUE;
  const int DEV_IDX_BRG = 1;
  const int PIPE_IDX_BRG = 2;
  const int DEV_IDX_NIR = 0;
  const int PIPE_IDX_NIR = 0;

  const Size VPSS_CH_SIZES_BGR[3] = {
      {1920, 1080}, {1080, 704}, {320, 224}};  // larger small
  const Size VPSS_CH_SIZES_NIR[3] = {
      {1920, 1080}, {1080, 704}, {320, 224}};  // larger small
  const int CH_INDEXES_BGR[3] = {0, 1, 2};
  const ROTATION_E CH_ROTATES_BGR[3] = {
      ROTATION_E::ROTATION_0,
      ROTATION_E::ROTATION_90,
      ROTATION_E::ROTATION_90,
  };
  const int CH_INDEXES_NIR[3] = {0, 1, 2};
  const ROTATION_E CH_ROTATES_NIR[3] = {
      ROTATION_E::ROTATION_0,
      ROTATION_E::ROTATION_90,
      ROTATION_E::ROTATION_90,
  };

  void run();
  bool capture_frame(ImagePackage *pkg);

  bool rx_finished_;

  Vi *vi_bgr_, *vi_nir_;
  Vpss *vpss_bgr_, *vpss_nir_;
  Vi_Vpss *vi_vpss_bgr_, *vi_vpss_nir_;

  ImagePackage *buffer_ping_, *buffer_pang_;
  PingPangBuffer<ImagePackage> *pingpang_buffer_;

  LCD_SCREEN_TYPE lcd_screen_type_ = SML_LCD_MIPI_8INCH_800X1280;
};

}  // namespace suanzi

#endif
