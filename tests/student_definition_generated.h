// automatically generated by the FlatBuffers compiler, do not modify

#ifndef FLATBUFFERS_GENERATED_STUDENTDEFINITION_H_
#define FLATBUFFERS_GENERATED_STUDENTDEFINITION_H_

#include "flatbuffers/flatbuffers.h"

struct Score;

struct Student;

struct Score FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  enum {
    VT_SUBJECT = 4,
    VT_SCORE = 6
  };
  const flatbuffers::String *subject() const { return GetPointer<const flatbuffers::String *>(VT_SUBJECT); }
  float score() const { return GetField<float>(VT_SCORE, 0.0f); }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<flatbuffers::uoffset_t>(verifier, VT_SUBJECT) &&
           verifier.Verify(subject()) &&
           VerifyField<float>(verifier, VT_SCORE) &&
           verifier.EndTable();
  }
};

struct ScoreBuilder {
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_subject(flatbuffers::Offset<flatbuffers::String> subject) { fbb_.AddOffset(Score::VT_SUBJECT, subject); }
  void add_score(float score) { fbb_.AddElement<float>(Score::VT_SCORE, score, 0.0f); }
  ScoreBuilder(flatbuffers::FlatBufferBuilder &_fbb) : fbb_(_fbb) { start_ = fbb_.StartTable(); }
  ScoreBuilder &operator=(const ScoreBuilder &);
  flatbuffers::Offset<Score> Finish() {
    auto o = flatbuffers::Offset<Score>(fbb_.EndTable(start_, 2));
    return o;
  }
};

inline flatbuffers::Offset<Score> CreateScore(flatbuffers::FlatBufferBuilder &_fbb,
    flatbuffers::Offset<flatbuffers::String> subject = 0,
    float score = 0.0f) {
  ScoreBuilder builder_(_fbb);
  builder_.add_score(score);
  builder_.add_subject(subject);
  return builder_.Finish();
}

inline flatbuffers::Offset<Score> CreateScoreDirect(flatbuffers::FlatBufferBuilder &_fbb,
    const char *subject = nullptr,
    float score = 0.0f) {
  return CreateScore(_fbb, subject ? _fbb.CreateString(subject) : 0, score);
}

struct Student FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  enum {
    VT_NAME = 4,
    VT_BIRTH_YEAR = 6,
    VT_MALE = 8,
    VT_SCORES = 10,
    VT_AVG_SCORES = 12,
    VT_SCHOOL_DAY = 14
  };
  const flatbuffers::String *name() const { return GetPointer<const flatbuffers::String *>(VT_NAME); }
  int16_t birth_year() const { return GetField<int16_t>(VT_BIRTH_YEAR, 0); }
  bool male() const { return GetField<uint8_t>(VT_MALE, 0) != 0; }
  const flatbuffers::Vector<flatbuffers::Offset<Score>> *scores() const { return GetPointer<const flatbuffers::Vector<flatbuffers::Offset<Score>> *>(VT_SCORES); }
  const Score *avg_scores() const { return GetPointer<const Score *>(VT_AVG_SCORES); }
  const flatbuffers::Vector<int8_t> *school_day() const { return GetPointer<const flatbuffers::Vector<int8_t> *>(VT_SCHOOL_DAY); }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<flatbuffers::uoffset_t>(verifier, VT_NAME) &&
           verifier.Verify(name()) &&
           VerifyField<int16_t>(verifier, VT_BIRTH_YEAR) &&
           VerifyField<uint8_t>(verifier, VT_MALE) &&
           VerifyField<flatbuffers::uoffset_t>(verifier, VT_SCORES) &&
           verifier.Verify(scores()) &&
           verifier.VerifyVectorOfTables(scores()) &&
           VerifyField<flatbuffers::uoffset_t>(verifier, VT_AVG_SCORES) &&
           verifier.VerifyTable(avg_scores()) &&
           VerifyField<flatbuffers::uoffset_t>(verifier, VT_SCHOOL_DAY) &&
           verifier.Verify(school_day()) &&
           verifier.EndTable();
  }
};

struct StudentBuilder {
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_name(flatbuffers::Offset<flatbuffers::String> name) { fbb_.AddOffset(Student::VT_NAME, name); }
  void add_birth_year(int16_t birth_year) { fbb_.AddElement<int16_t>(Student::VT_BIRTH_YEAR, birth_year, 0); }
  void add_male(bool male) { fbb_.AddElement<uint8_t>(Student::VT_MALE, static_cast<uint8_t>(male), 0); }
  void add_scores(flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<Score>>> scores) { fbb_.AddOffset(Student::VT_SCORES, scores); }
  void add_avg_scores(flatbuffers::Offset<Score> avg_scores) { fbb_.AddOffset(Student::VT_AVG_SCORES, avg_scores); }
  void add_school_day(flatbuffers::Offset<flatbuffers::Vector<int8_t>> school_day) { fbb_.AddOffset(Student::VT_SCHOOL_DAY, school_day); }
  StudentBuilder(flatbuffers::FlatBufferBuilder &_fbb) : fbb_(_fbb) { start_ = fbb_.StartTable(); }
  StudentBuilder &operator=(const StudentBuilder &);
  flatbuffers::Offset<Student> Finish() {
    auto o = flatbuffers::Offset<Student>(fbb_.EndTable(start_, 6));
    return o;
  }
};

inline flatbuffers::Offset<Student> CreateStudent(flatbuffers::FlatBufferBuilder &_fbb,
    flatbuffers::Offset<flatbuffers::String> name = 0,
    int16_t birth_year = 0,
    bool male = false,
    flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<Score>>> scores = 0,
    flatbuffers::Offset<Score> avg_scores = 0,
    flatbuffers::Offset<flatbuffers::Vector<int8_t>> school_day = 0) {
  StudentBuilder builder_(_fbb);
  builder_.add_school_day(school_day);
  builder_.add_avg_scores(avg_scores);
  builder_.add_scores(scores);
  builder_.add_name(name);
  builder_.add_birth_year(birth_year);
  builder_.add_male(male);
  return builder_.Finish();
}

inline flatbuffers::Offset<Student> CreateStudentDirect(flatbuffers::FlatBufferBuilder &_fbb,
    const char *name = nullptr,
    int16_t birth_year = 0,
    bool male = false,
    const std::vector<flatbuffers::Offset<Score>> *scores = nullptr,
    flatbuffers::Offset<Score> avg_scores = 0,
    const std::vector<int8_t> *school_day = nullptr) {
  return CreateStudent(_fbb, name ? _fbb.CreateString(name) : 0, birth_year, male, scores ? _fbb.CreateVector<flatbuffers::Offset<Score>>(*scores) : 0, avg_scores, school_day ? _fbb.CreateVector<int8_t>(*school_day) : 0);
}

inline const Student *GetStudent(const void *buf) { return flatbuffers::GetRoot<Student>(buf); }

inline bool VerifyStudentBuffer(flatbuffers::Verifier &verifier) { return verifier.VerifyBuffer<Student>(nullptr); }

inline void FinishStudentBuffer(flatbuffers::FlatBufferBuilder &fbb, flatbuffers::Offset<Student> root) { fbb.Finish(root); }

#endif  // FLATBUFFERS_GENERATED_STUDENTDEFINITION_H_
