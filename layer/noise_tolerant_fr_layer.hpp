// by huangyangyu, 2018/9/24
#ifndef CAFFE_NOISE_TOLERANT_FR_LAYER_HPP_
#define CAFFE_NOISE_TOLERANT_FR_LAYER_HPP_

#include <vector>
#include <queue>
#include <algorithm>

#include "caffe/blob.hpp"
#include "caffe/layer.hpp"
#include "caffe/proto/caffe.pb.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace caffe {

  template <typename Dtype>
  class NoiseTolerantFRLayer : public Layer<Dtype> {
  public:
    explicit NoiseTolerantFRLayer(const LayerParameter& param)
      : Layer<Dtype>(param) {}
    virtual void LayerSetUp(const vector<Blob<Dtype>*>& bottom,
                            const vector<Blob<Dtype>*>& top);
    virtual void Reshape(const vector<Blob<Dtype>*>& bottom,
                         const vector<Blob<Dtype>*>& top);

    virtual inline const char* type() const { return "NoiseTolerantFR"; }
    virtual inline int ExactNumTopBlobs() const { return 1; }
    //virtual inline int ExactNumBottomBlobs() const { return 3; }
    virtual inline int MinBottomBlobs() const { return 3; }
    virtual inline int MaxBottomBlobs() const { return 4; }

  protected:
    virtual void Forward_cpu(const vector<Blob<Dtype>*>& bottom,
                             const vector<Blob<Dtype>*>& top);
    //virtual void Forward_gpu(const vector<Blob<Dtype>*>& bottom,
    //                         const vector<Blob<Dtype>*>& top);

    virtual void Backward_cpu(const vector<Blob<Dtype>*>& top,
                              const vector<bool>& propagate_down, const vector<Blob<Dtype>*>& bottom);
    //virtual void Backward_gpu(const vector<Blob<Dtype>*>& top,
    //                          const vector<bool>& propagate_down, const vector<Blob<Dtype>*>& bottom);

    Dtype cos2weight(const Dtype cos);
    int get_bin_id(const Dtype cos);
    Dtype get_cos(const int bin_id);

    bool shield_forward_;// 1
    int start_iter_;// 2
    int bins_;// 3
    int slide_batch_num_;// 4
    Dtype value_low_;// 5
    Dtype value_high_;// 6
    bool debug_;// 7
    std::string debug_prefix_;// 8

    int iter_;
    bool skip_;
    Dtype noise_ratio_;
    std::queue<int> queue_label_;
    std::queue<int> queue_bin_id_;
    std::vector<Dtype> pdf_;
    std::vector<Dtype> clean_pdf_;
    std::vector<Dtype> noise_pdf_;
    std::vector<Dtype> pcf_;
    int l_bin_id_;// left end point
    int r_bin_id_;// right end point
    int lt_bin_id_;// left extreme point
    int rt_bin_id_;// right extreme point
    std::vector<int> t_bin_ids_;// extreme points
    Blob<Dtype> weights_;

    //80:1.282, 90:1.645, 95:1.960, 99:2.576
    const Dtype s = 1.0-0.99;
    const Dtype r = 2.576;

    // filter radius-1
    const int fr = 2;
  };

}  // namespace caffe

#endif  // CAFFE_NOISE_TOLERANT_FR_LAYER_HPP_
