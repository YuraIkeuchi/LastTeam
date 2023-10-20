#pragma once
// 参考ページ
#include <functional>
#include <assert.h>
namespace yEngine
{

    enum class AccessorType // Accessタイプ
    {
        ReadOnly = 0x01,
        WriteOnly = 0x02,
        AllAccess = 0x03,
    };

    template<class ReferenceT>
    class Property
    {
    public:
        Property() = default;
        //Property(const Property &other) {  };
        Property(
            ReferenceT *r = nullptr,
            AccessorType type = AccessorType::AllAccess,
            std::function<ReferenceT()> get = nullptr,
            std::function<void(ReferenceT value)> set = nullptr) :
            r_(*r),
            access_type_(type),
            get_(get),
            set_(set)
        {
        }
        // ゲッター
        operator ReferenceT() const
        {
            // 書き込み専用ならエラーを返す
            //assert(static_cast<int>(AccessorType::WriteOnly) & static_cast<int>(access_type_));
            // getが設定されてれば登録されたほうを呼ぶ
            return get_ ? this->get_() : this->r_;
        }

        // 直接中身を参照できるようにアロー演算子もオーバーロード
        ReferenceT *operator ->() const
        {

            return get_ ? &this->get_() : &this->r_;
        }

        // 参照渡しオーバーロード
        ReferenceT *operator &()const
        {
            return &r_;
        }

        // セッター
        void operator =(const ReferenceT v)
        {
            // 読み込み専用ならエラーを返す
            assert(static_cast<int>(AccessorType::ReadOnly) != static_cast<int>(access_type_));
            if (set_) // setが設定されていれば登録されたほうを呼ぶ
            {
                this->set_(v);
            }
            else
            {
                r_ = v;
            }
        }

        ReferenceT operator -() const
        {
            return get_ ? -this->get_() : -r_;
        }
        ReferenceT operator +(const ReferenceT &v) const
        {
            return get_ ? this->get_() + v : r_ + v;
        }
        ReferenceT &operator +=(const ReferenceT &v) 
        {
            if(set_ != nullptr)
            {
                set_(*this + v);
                return r_;
            }
            return r_ += v;
        }
        ReferenceT operator -(const ReferenceT &v) const
        {
            return get_ ? this->get_() - v : r_ - v;
        }
		ReferenceT &operator -=(const ReferenceT &v) 
        {
            if (set_ != nullptr)
            {
                set_(*this - v);
                return r_;
            }
            return r_ -= v;
        }
        // 比較演算子
        bool operator ==(const ReferenceT &v) const
        {
			return get_ ? (this->get_() == v) : (this->r_ == v);
        }
        bool operator !=(const ReferenceT &v) const
        {
            return get_ ? !(this->get_() == v) : !(this->r_ == v);
        }
        bool operator !() const
        {
            return get_ ? !(this->get_()) : !(this->r_);
        }


        ReferenceT &r_;                                           // 値
        AccessorType access_type_;                                // アクセスタイプ
        std::function<ReferenceT()> get_ = nullptr;               // ゲット時の処理
        std::function<void(ReferenceT value)> set_ = nullptr;     // セット時の処理

    };
}